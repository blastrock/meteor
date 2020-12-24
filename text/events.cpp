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

#include "events.hpp"
#include "MainText.hpp"
#include <ameteor/core.hpp>

namespace mym
{
Events::Events(sf::Window& window, MainText* m)
  : m_window(window), m_maintext(m)
{
}

void Events::InitAMeteor(AMeteor::Core& core)
{
  m_core = &core;
  core.get<AMeteor::Lcd>().sig_vblank = [this] { CheckEvents(); };
}

void Events::CheckEvents()
{
  uint16_t input = 0;
#define FETCH(key, btn)                                       \
  input |= sf::Keyboard::isKeyPressed(sf::Keyboard::Key::key) \
               ? AMeteor::Keypad::BTN_##btn                   \
               : 0;
  FETCH(A, A);
  FETCH(B, B);
  FETCH(Backspace, SELECT);
  FETCH(Enter, START);
  FETCH(Right, RIGHT);
  FETCH(Left, LEFT);
  FETCH(Up, UP);
  FETCH(Down, DOWN);
  FETCH(R, R);
  FETCH(L, L);
#undef FETCH

  input ^= 0x3FF;

  m_core->get<AMeteor::Keypad>().SetPadState(input);

  sf::Event event;
  while (m_window.pollEvent(event))
  {
    switch (event.type)
    {
    case sf::Event::KeyPressed:
      if (event.key.code == sf::Keyboard::Key::F1)
        m_maintext->PushLoadState(0);
      else if (event.key.code == sf::Keyboard::Key::F3)
        m_maintext->PushSaveState(0);
      break;
    case sf::Event::Closed:
      m_maintext->Stop();
      break;
    default:
      break;
    }
  }
}
}
