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

#ifndef __KEYPAD_H__
#define __KEYPAD_H__

#include <stdint.h>

namespace sf
{
	class Window;
}

namespace AMeteor
{
	class Keypad
	{
		public :
			Keypad ();
			~Keypad ();

			void Reset ()
			{
			}

			bool KeyPressed(int code);
			bool KeyReleased(int code);
			bool JoyButtonPressed (unsigned int joyid, unsigned int button);
			bool JoyButtonReleased (unsigned int joyid, unsigned int button);
			bool JoyMoved (unsigned int joyid, unsigned int axis, float pos);

			void VBlank ();

		private :
			uint16_t& m_keyinput;
			uint16_t& m_keycnt;
	};
}

#endif
