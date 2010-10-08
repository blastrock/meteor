// Meteor - A Nintendo Gameboy Advance emulator
// Copyright (C) 2009 Philippe Daouadi
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

#include "ameteor/events.hpp"
#include "globals.hpp"
#include "ameteor.hpp"
#include <SFML/Window.hpp>

namespace AMeteor
{
	namespace Events
	{
		void CheckEvents (sf::Window& win)
		{
			// FIXME
			// when using the gtkmm frontend, we can't GetEvent() on an sf window
			// without waiting some time
			// this dirty fix works, but we have to know why it crashes and how to
			// fix it cleanly
			static int i = 0;
			if (i++ < 2)
				return;

			sf::Event event;
			while (win.GetEvent(event))
			{
				switch (event.Type)
				{
					case sf::Event::KeyPressed:
						KEYPAD.KeyPressed(event.Key.Code);
						break;
					case sf::Event::KeyReleased:
						KEYPAD.KeyReleased(event.Key.Code);
						break;
					case sf::Event::JoyButtonPressed:
						switch (event.JoyButton.Button)
						{
							case 7:
								LCD.SetFrameskip(SPDUP_FRMSKIP);
								SOUND.SetSampleskip(SPDUP_SNDSKIP);
								break;
							default:
								KEYPAD.JoyButtonPressed(event.JoyButton.JoystickId,
										event.JoyButton.Button);
								break;
						}
						break;
					case sf::Event::JoyButtonReleased:
						switch (event.JoyButton.Button)
						{
							case 7:
								LCD.SetFrameskip(0);
								SOUND.SetSampleskip(0);
								break;
							default:
								KEYPAD.JoyButtonReleased(event.JoyButton.JoystickId,
										event.JoyButton.Button);
								break;
						}
						break;
					case sf::Event::JoyMoved:
						KEYPAD.JoyMoved(event.JoyMove.JoystickId,
								event.JoyMove.Axis, event.JoyMove.Position);
						break;
					case sf::Event::Resized:
						LCD.EventResize(event.Size.Width, event.Size.Height);
						break;
					case sf::Event::Closed:
						CPU.Stop();
						break;
					default:
						break;
				}
			}
		}
	}
}
