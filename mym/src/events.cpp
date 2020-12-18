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

#include "mym/events.hpp"
#include <ameteor.hpp>

namespace mym
{
Events::Events(sf::Window& window) : m_window(window)
{
}

void Events::InitAMeteor(AMeteor::Core& core)
{
  m_core = &core;
  core.get<AMeteor::Lcd>().sig_vblank.connect(
      syg::mem_fun(*this, &Events::CheckEvents));
}

void Events::CheckEvents()
{
  sf::Event event;
  while (m_window.pollEvent(event))
  {
    switch (event.type)
    {
    case sf::Event::KeyPressed:
      m_core->get<AMeteor::Keypad>().KeyPressed(event.key.code);
      break;
    case sf::Event::KeyReleased:
      m_core->get<AMeteor::Keypad>().KeyReleased(event.key.code);
      break;
    case sf::Event::JoystickButtonPressed:
      switch (event.joystickButton.button)
      {
      // XXX
      // case 7:
      //	LCD.SetFrameskip(SPDUP_FRMSKIP);
      //	SOUND.SetSampleskip(SPDUP_SNDSKIP);
      //	break;
      default:
        m_core->get<AMeteor::Keypad>().JoyButtonPressed(
            event.joystickButton.joystickId, event.joystickButton.button);
        break;
      }
      break;
    case sf::Event::JoystickButtonReleased:
      switch (event.joystickButton.button)
      {
      // XXX
      // case 7:
      //	LCD.SetFrameskip(0);
      //	SOUND.SetSampleskip(0);
      //	break;
      default:
        m_core->get<AMeteor::Keypad>().JoyButtonReleased(
            event.joystickButton.joystickId, event.joystickButton.button);
        break;
      }
      break;
    case sf::Event::JoystickMoved:
      m_core->get<AMeteor::Keypad>().JoyMoved(
          event.joystickMove.joystickId,
          event.joystickMove.axis,
          event.joystickMove.position);
      break;
    // case sf::Event::Resized:
    //	LCD.EventResize(event.Size.Width, event.Size.Height);
    //	break;
    case sf::Event::Closed:
      m_core->get<AMeteor::Cpu>().Stop();
      break;
    default:
      break;
    }
  }
}
}
