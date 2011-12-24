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
				sigc::mem_fun(*this, &Events::CheckEvents));
	}

	void Events::CheckEvents ()
	{
		sf::Event event;
		while (m_window.GetEvent(event))
		{
			switch (event.Type)
			{
				case sf::Event::KeyPressed:
					AMeteor::_keypad.KeyPressed(event.Key.Code);
					break;
				case sf::Event::KeyReleased:
					AMeteor::_keypad.KeyReleased(event.Key.Code);
					break;
				case sf::Event::JoyButtonPressed:
					switch (event.JoyButton.Button)
					{
						// XXX
						//case 7:
						//	LCD.SetFrameskip(SPDUP_FRMSKIP);
						//	SOUND.SetSampleskip(SPDUP_SNDSKIP);
						//	break;
						default:
							AMeteor::_keypad.JoyButtonPressed(event.JoyButton.JoystickId,
									event.JoyButton.Button);
							break;
					}
					break;
				case sf::Event::JoyButtonReleased:
					switch (event.JoyButton.Button)
					{
						// XXX
						//case 7:
						//	LCD.SetFrameskip(0);
						//	SOUND.SetSampleskip(0);
						//	break;
						default:
							AMeteor::_keypad.JoyButtonReleased(event.JoyButton.JoystickId,
									event.JoyButton.Button);
							break;
					}
					break;
				case sf::Event::JoyMoved:
					AMeteor::_keypad.JoyMoved(event.JoyMove.JoystickId,
							event.JoyMove.Axis, event.JoyMove.Position);
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
