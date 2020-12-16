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

#ifndef __MYM_WINDOW_H__
#define __MYM_WINDOW_H__

#include <GL/glew.h>

#include <GL/gl.h>
#include <SFML/Graphics/RenderWindow.hpp>

namespace mym
{
class Window
{
public:
  Window();
  ~Window();

  inline sf::Window& GetWindow();

  void InitAMeteor();
  void Init(sf::WindowHandle display = 0);
  void Uninit();

  void ShowFrame(const uint16_t* frame);

private:
  volatile uint16_t* m_tbase;

  pthread_t m_thread;
  pthread_mutex_t m_mutex;
  pthread_cond_t m_cond;
  volatile bool m_quit;

  sf::RenderWindow m_window;

  GLuint m_pbo;
  GLuint m_texture;
  GLuint m_vbo;

  void InitGl();
  void UninitGl();
  void StartThread();
  void StopThread();

  static void* EntryPoint(void* ptr);
  void MainLoop();
};

inline sf::Window& Window::GetWindow()
{
  return m_window;
}
}

#endif
