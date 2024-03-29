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

#include "window.hpp"
#include <ameteor/core.hpp>
#include <cstring>
#include <sched.h>
#include <stdio.h>
#include <sys/syscall.h>
#include <unistd.h>

namespace mym
{
Window::Window()
  : m_tbase(new uint16_t[240 * 160])
  , m_thread(0)
  , m_pbo(0)
  , m_texture(0)
  , m_vbo(0)
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

Window::~Window()
{
  Uninit();

  // TODO check for errors
  pthread_cond_destroy(&m_cond);
  pthread_mutex_destroy(&m_mutex);

  delete[] m_tbase;
}

void Window::InitAMeteor(AMeteor::Core& core)
{
  core.get<AMeteor::Lcd>().GetScreen().GetRenderer().sig_frame =
      [this](auto frame) { ShowFrame(frame); };
}

// TODO check if we crash if we don't Init()
void Window::Init(sf::WindowHandle display)
{
  Uninit();

  if (display)
    m_window.create(display);
  else
    m_window.create(sf::VideoMode(240, 160, 32), "Meteor");

  InitGl();
  StartThread();
}

void Window::Uninit()
{
  StopThread();
  UninitGl();
}

void Window::InitGl()
{
  static const int Square[] = {
      0,
      0,
      1,
      0,
      1,
      1,
      0,
      1,
  };

  m_window.setActive();

  // TODO check (may be called multiple times)
  if (GLEW_OK != glewInit())
    puts("err");

  // glEnable(GL_DEBUG_OUTPUT);
  // glDebugMessageCallback( []( GLenum source,
  // 			GLenum type,
  // 			GLuint id,
  // 			GLenum severity,
  // 			GLsizei length,
  // 			const GLchar* message,
  // 			const void* userParam )
  // 		{
  // 		fprintf( stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x,
  // message = %s\n", 				( type == GL_DEBUG_TYPE_ERROR ?
  // "** GL ERROR
  // **" : "" ), 				type, severity, message );
  // 		}, 0 );
  glEnable(GL_TEXTURE_2D);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0, 1, 1, 0, -1, 1);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  glGenBuffers(1, &m_vbo);
  glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Square), Square, GL_STATIC_DRAW);

  glGenTextures(1, &m_texture);
  glBindTexture(GL_TEXTURE_2D, m_texture);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexImage2D(
      GL_TEXTURE_2D, 0, GL_RGB5, 240, 160, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

  glGenBuffers(1, &m_pbo);
  glBindBuffer(GL_PIXEL_UNPACK_BUFFER, m_pbo);
  glBufferData(GL_PIXEL_UNPACK_BUFFER, 2 * 240 * 160, NULL, GL_DYNAMIC_DRAW);

  m_window.display();
  m_window.setActive(false);
}

void Window::StartThread()
{
  m_quit = false;
  int ret = pthread_create(&m_thread, NULL, EntryPoint, this);
  if (ret)
  {
    puts("Error: cannot create renderer thread");
    abort();
  }
}

void Window::StopThread()
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

void Window::UninitGl()
{
  if (m_window.setActive())
  {
    glDeleteBuffers(1, &m_pbo);
    glDeleteTextures(1, &m_texture);
    glDeleteBuffers(1, &m_vbo);
    m_vbo = m_texture = m_pbo = 0;
    m_window.close();
  }
}

void Window::ShowFrame(const uint16_t* frame)
{
  // TODO check for errors
  pthread_mutex_lock(&m_mutex);
  // const_cast removes volatile
  std::memcpy(const_cast<uint16_t*>(m_tbase), frame, 240 * 160 * 2);
  pthread_cond_signal(&m_cond);
  pthread_mutex_unlock(&m_mutex);
}

void* Window::EntryPoint(void* ptr)
{
  static_cast<Window*>(ptr)->MainLoop();
  return NULL;
}

void Window::MainLoop()
{
  {
    cpu_set_t set;
    CPU_ZERO(&set);
    CPU_SET(0, &set);
    sched_setaffinity(syscall(__NR_gettid), sizeof(set), &set);
  }

  if (!m_window.setActive())
    puts("Can't activate window !");

  glViewport(0, 0, 240, 160);

  void* buf;
  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_TEXTURE_COORD_ARRAY);
  glTexCoordPointer(2, GL_INT, 2 * sizeof(int), (void*)0);
  glVertexPointer(2, GL_INT, 2 * sizeof(int), (void*)0);

  // TODO check for errors
  pthread_mutex_lock(&m_mutex);
  while (!m_quit)
  {
    // this unlocks the mutex while waiting
    pthread_cond_wait(&m_cond, &m_mutex);

    buf = glMapBuffer(GL_PIXEL_UNPACK_BUFFER, GL_WRITE_ONLY);
    memcpy((void*)buf, (void*)m_tbase, 240 * 160 * 2);
    glUnmapBuffer(GL_PIXEL_UNPACK_BUFFER);

    glTexSubImage2D(
        GL_TEXTURE_2D,
        0,
        0,
        0,
        240,
        160,
        GL_RGBA,
        GL_UNSIGNED_SHORT_1_5_5_5_REV,
        0);

    glDrawArrays(GL_QUADS, 0, 4);

    m_window.display();
  }

  glDisableClientState(GL_VERTEX_ARRAY);
  glDisableClientState(GL_TEXTURE_COORD_ARRAY);

  m_window.setActive(false);
  pthread_mutex_unlock(&m_mutex);
}
}
