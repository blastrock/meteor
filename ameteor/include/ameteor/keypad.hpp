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

#ifndef __KEYPAD_H__
#define __KEYPAD_H__

#include <map>
#include <stdint.h>

namespace AMeteor
{
class Keypad
{
public:
  enum Button
  {
    BTN_A = 0x001,
    BTN_B = 0x002,
    BTN_SELECT = 0x004,
    BTN_START = 0x008,
    BTN_RIGHT = 0x010,
    BTN_LEFT = 0x020,
    BTN_UP = 0x040,
    BTN_DOWN = 0x080,
    BTN_R = 0x100,
    BTN_L = 0x200
  };

  Keypad();

  void Reset()
  {
  }

  inline void SetPadState(uint16_t keys);

  void VBlank();

private:
  uint16_t& m_keyinput;
  uint16_t& m_keycnt;
};

void Keypad::SetPadState(uint16_t keys)
{
  m_keyinput = keys;
}
}

#endif
