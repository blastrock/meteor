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

#include "ameteor/keypad.hpp"
#include "ameteor/core.hpp"
#include "globals.hpp"

namespace AMeteor
{
Keypad::Keypad()
  : m_keyinput(IO.GetRef16(Io::KEYINPUT)), m_keycnt(IO.GetRef16(Io::KEYCNT))
{
}

void Keypad::VBlank()
{
  // if keypad IRQ are enabled
  if (m_keycnt & (0x1 << 14))
  {
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
}
