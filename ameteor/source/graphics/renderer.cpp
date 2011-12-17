// Meteor - A Nintendo Gameboy Advance emulator
// Copyright (C) 2009-2011 Philippe Daouadi
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include "ameteor/graphics/renderer.hpp"
#include "filters/hq4x.hpp"
#include <cstring>
#include <sys/syscall.h>
#include <sched.h>
#include <X11/Xlib.h>
// TODO try to remove this include
#include <stdio.h>

namespace AMeteor
{
	namespace Graphics
	{
		Renderer::Renderer (const uint16_t* surface) :
			sf::RenderWindow (),
			m_base(surface),
			m_tbase(new uint16_t[240*160]),
			m_w(0),
			m_h(0),
			m_thread(0),
			m_pbo(0),
			m_texture(0),
			m_vbo(0)
		{
			cpu_set_t set;
			CPU_ZERO(&set);
			CPU_SET(1, &set);
			sched_setaffinity(syscall(__NR_gettid), sizeof(set), &set);

			int ret = pthread_mutex_init(&m_mutex, NULL);
			if (ret)
			{
				puts("Error: cannot create renderer mutex");
				abort();
			}
			ret = pthread_cond_init(&m_cond, NULL);
			if (ret)
			{
				puts("Error: cannot create renderer condition variable");
				abort();
			}
		}

		Renderer::~Renderer ()
		{
			Uninit();

			// TODO check for errors
			pthread_cond_destroy(&m_cond);
			pthread_mutex_destroy(&m_mutex);

			delete [] m_tbase;
		}

		// TODO check if we crash if we don't Init()
		void Renderer::Init(sf::WindowHandle display)
		{
			Uninit();

			if (display)
				this->Create (display);
			else
				this->Create (sf::VideoMode(4*240, 4*160, 32), "Meteor");

			InitGl();
			StartThread();
		}

		void Renderer::Uninit()
		{
			StopThread();
			UninitGl();
		}

		void Renderer::InitGl()
		{
			static const int Square[] = {
				0, 0,
				1, 0,
				1, 1,
				0, 1,
			};

			this->SetActive();

			// TODO check (may be called multiple times)
			if (GLEW_OK != glewInit())
				puts("err");

			glEnable(GL_TEXTURE_2D);

			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			glOrtho(0, 1, 1, 0, -1, 1);

			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();

			glGenBuffers(1, &m_vbo);
			glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
			glBufferData(GL_ARRAY_BUFFER, sizeof(Square), Square, GL_STATIC_DRAW);

			glGenTextures (1, &m_texture);
			glBindTexture (GL_TEXTURE_2D, m_texture);
			glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 4*240, 4*160, 0, GL_BGRA,
					GL_BYTE, NULL);

			glGenBuffers (1, &m_pbo);
			glBindBuffer (GL_PIXEL_UNPACK_BUFFER, m_pbo);
			glBufferData(GL_PIXEL_UNPACK_BUFFER, 4*240*4*160*4, NULL,
					GL_DYNAMIC_DRAW);

			this->Display();
			this->SetActive(false);
		}

		void Renderer::StartThread()
		{
			m_quit = false;
			int ret = pthread_create(&m_thread, NULL, EntryPoint, this);
			if (ret)
			{
				puts("Error: cannot create renderer thread");
				abort();
			}
		}

		void Renderer::StopThread()
		{
			if (m_thread)
			{
				m_quit = true;
				// TODO check for errors
				// we send the signal to make the thread continue and quit
				pthread_cond_signal(&m_cond);
				pthread_join(m_thread, NULL);
				// FIXME ? is this safe ? is the m_thread still allocated ?
				m_thread = 0;
			}
		}

		void Renderer::UninitGl()
		{
			if (this->SetActive())
			{
				glDeleteBuffers(1, &m_pbo);
				glDeleteTextures(1, &m_texture);
				glDeleteBuffers(1, &m_vbo);
				m_vbo = m_texture = m_pbo = 0;
				this->Close();
			}
		}

		void Renderer::VBlank ()
		{
			// TODO check for errors
			pthread_mutex_lock(&m_mutex);
			// const_cast removes volatile
			std::memcpy(const_cast<uint16_t*>(m_tbase), m_base, 240*160*2);
			pthread_cond_signal(&m_cond);
			pthread_mutex_unlock(&m_mutex);
		}

		void Renderer::EventResize (unsigned int w, unsigned int h)
		{
			m_w = w; m_h = h;
		}

		void* Renderer::EntryPoint (void* ptr)
		{
			static_cast<Renderer*>(ptr)->MainLoop();
			return NULL;
		}

		void Renderer::MainLoop ()
		{
			{
				cpu_set_t set;
				CPU_ZERO(&set);
				CPU_SET(0, &set);
				sched_setaffinity(syscall(__NR_gettid), sizeof(set), &set);
			}

			if (!this->SetActive())
				puts("Can't activate window !");

			void* buf;

			// TODO check for errors
			pthread_mutex_lock(&m_mutex);
			while (!m_quit)
			{
				// this unlocks the mutex while waiting
				pthread_cond_wait(&m_cond, &m_mutex);

				if (m_w && m_h)
				{
					glViewport(0, 0, m_w, m_h);
					m_w = m_h = 0;
				}

				buf = glMapBuffer(GL_PIXEL_UNPACK_BUFFER, GL_WRITE_ONLY);

#if 1
				Filters::hq4x((unsigned char*)m_tbase, (unsigned char*)buf,
						240, 160, 240*4*4);
#else
				memcpy((void*)buf, (void*)m_tbase, 240*160*2);
#endif

				glUnmapBuffer(GL_PIXEL_UNPACK_BUFFER);

				glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 240*4, 160*4, GL_BGRA,
						GL_UNSIGNED_BYTE, 0);

				glEnableClientState(GL_VERTEX_ARRAY);
				glEnableClientState(GL_TEXTURE_COORD_ARRAY);
				glTexCoordPointer(2, GL_INT, 2*sizeof(int), (void*)0);
				glVertexPointer(2, GL_INT, 2*sizeof(int), (void*)0);
				glDrawArrays(GL_QUADS, 0, 4);
				glDisableClientState(GL_VERTEX_ARRAY);
				glDisableClientState(GL_TEXTURE_COORD_ARRAY);

				glFlush();

				this->Display();
			}

			this->SetActive(false);
			pthread_mutex_unlock(&m_mutex);
		}
	}
}
