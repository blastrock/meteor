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

	//XXX remove that ?
	Keypad::~Keypad ()
	{
	}

	bool Keypad::KeyPressed(int code)
	{
		switch (code)
		{
#define CHECKDOWN(key, val) \
	case key: \
		m_keyinput &= ~((uint16_t)val); \
		return true
			CHECKDOWN('w',            0x001);
			CHECKDOWN('x',            0x002);
			CHECKDOWN('z',            0x004);
			CHECKDOWN('a',            0x008);
			CHECKDOWN(sf::Key::Right, 0x010);
			CHECKDOWN(sf::Key::Left,  0x020);
			CHECKDOWN(sf::Key::Up,    0x040);
			CHECKDOWN(sf::Key::Down,  0x080);
			CHECKDOWN('s',            0x100);
			CHECKDOWN('q',            0x200);
			default:
				return false;
#undef CHECKDOWN
		}
	}

	bool Keypad::KeyReleased(int code)
	{
		switch (code)
		{
#define CHECKUP(key, val) \
	case key: \
		m_keyinput |= val; \
		return true
			CHECKUP('w',            0x001);
			CHECKUP('x',            0x002);
			CHECKUP('z',            0x004);
			CHECKUP('a',            0x008);
			CHECKUP(sf::Key::Right, 0x010);
			CHECKUP(sf::Key::Left,  0x020);
			CHECKUP(sf::Key::Up,    0x040);
			CHECKUP(sf::Key::Down,  0x080);
			CHECKUP('s',            0x100);
			CHECKUP('q',            0x200);
			default:
				return false;
#undef CHECKUP
		}
	}

	bool Keypad::JoyButtonPressed (unsigned int joyid, unsigned int button)
	{
		switch (button)
		{
#define CHECKDOWN(key, val) \
	case key: \
		m_keyinput &= ~((uint16_t)val); \
		return true
			CHECKDOWN(2, 0x001);
			CHECKDOWN(0, 0x002);
			CHECKDOWN(8, 0x004);
			CHECKDOWN(9, 0x008);
			CHECKDOWN(6, 0x100);
			CHECKDOWN(4, 0x200);
			default:
				return false;
#undef CHECKDOWN
		}
	}

	bool Keypad::JoyButtonReleased (unsigned int joyid, unsigned int button)
	{
		switch (button)
		{
#define CHECKUP(key, val) \
	case key: \
		m_keyinput |= val; \
		return true
			CHECKUP(2, 0x001);
			CHECKUP(0, 0x002);
			CHECKUP(8, 0x004);
			CHECKUP(9, 0x008);
			CHECKUP(6, 0x100);
			CHECKUP(4, 0x200);
			default:
				return false;
#undef CHECKUP
		}
	}

	bool Keypad::JoyMoved (unsigned int joyid, unsigned int axis, float pos)
	{
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
