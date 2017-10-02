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
	Events::Events(sf::Window& window) :
		m_window(window)
	{
	}

	void Events::InitAMeteor()
	{
		AMeteor::_lcd.sig_vblank.connect(
				syg::mem_fun(*this, &Events::CheckEvents));
	}

	void Events::CheckEvents ()
	{
		sf::Event event;
		while (m_window.pollEvent(event))
		{
			switch (event.type)
			{
				case sf::Event::KeyPressed:
					AMeteor::_keypad.KeyPressed(event.key.code);
					break;
				case sf::Event::KeyReleased:
					AMeteor::_keypad.KeyReleased(event.key.code);
					break;
				case sf::Event::JoystickButtonPressed:
					switch (event.joystickButton.button)
					{
						// XXX
						//case 7:
						//	LCD.SetFrameskip(SPDUP_FRMSKIP);
						//	SOUND.SetSampleskip(SPDUP_SNDSKIP);
						//	break;
						default:
							AMeteor::_keypad.JoyButtonPressed(event.joystickButton.joystickId,
									event.joystickButton.button);
							break;
					}
					break;
				case sf::Event::JoystickButtonReleased:
					switch (event.joystickButton.button)
					{
						// XXX
						//case 7:
						//	LCD.SetFrameskip(0);
						//	SOUND.SetSampleskip(0);
						//	break;
						default:
							AMeteor::_keypad.JoyButtonReleased(event.joystickButton.joystickId,
									event.joystickButton.button);
							break;
					}
					break;
				case sf::Event::JoystickMoved:
					AMeteor::_keypad.JoyMoved(event.joystickMove.joystickId,
							event.joystickMove.axis, event.joystickMove.position);
					break;
				//case sf::Event::Resized:
				//	LCD.EventResize(event.Size.Width, event.Size.Height);
				//	break;
				case sf::Event::Closed:
					AMeteor::_cpu.Stop();
					break;
				default:
					break;
			}
		}
	}
}
