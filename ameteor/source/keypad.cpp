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

#include "ameteor/keypad.hpp"
#include "globals.hpp"
#include "ameteor.hpp"

namespace AMeteor
{
	Keypad::Keypad () :
		m_keyinput(IO.GetRef16(Io::KEYINPUT)),
		m_keycnt(IO.GetRef16(Io::KEYCNT))
	{
	}

	void Keypad::KeyPressed(int code)
	{
		if (m_keys.count(code))
			m_keyinput &= ~m_keys[code];
	}

	void Keypad::KeyReleased(int code)
	{
		if (m_keys.count(code))
			m_keyinput |= m_keys[code];
	}

	void Keypad::JoyButtonPressed (uint16_t joyid, uint16_t button)
	{
		uint32_t id = ((int)joyid) << 16 | button;
		if (m_joys.count(id))
			m_keyinput &= ~m_joys[id];
	}

	void Keypad::JoyButtonReleased (uint16_t joyid, uint16_t button)
	{
		uint32_t id = ((int)joyid) << 16 | button;
		if (m_joys.count(id))
			m_keyinput |= m_joys[id];
	}

	bool Keypad::JoyMoved (unsigned int joyid, unsigned int axis, float pos)
	{
		/* TODO
		switch (axis)
		{
			case 4:
				if (pos < 0)
					m_keyinput &= ~((uint16_t)0x020);
				else if (pos > 0)
					m_keyinput &= ~((uint16_t)0x010);
				else
					m_keyinput |= 0x030;
				return true;
			case 5:
				if (pos < 0)
					m_keyinput &= ~((uint16_t)0x040);
				else if (pos > 0)
					m_keyinput &= ~((uint16_t)0x080);
				else
					m_keyinput |= 0x0C0;
				return true;
			default:
				return false;
		}
		*/
		return false;
	}

	void Keypad::VBlank ()
	{
		// if keypad IRQ are enabled
		if (m_keycnt & (0x1 << 14))
			// if irq condition is and
			if (m_keycnt & (0x1 << 15))
			{
				// if condition is satisfied
				if ((~m_keyinput & m_keycnt & 0x3FF) == (m_keycnt & 0x3FF))
					CPU.SendInterrupt(0x1000);
			}
			// if irq condition is or
			else
			{
				// if condition is satisfied
				if (~m_keyinput & m_keycnt & 0x3FF)
					CPU.SendInterrupt(0x1000);
			}
	}
}
