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

#include <stdint.h>

inline uint32_t COLORS_16_32(uint16_t val)
{
	return
		((((val & 0x001F) * 255)/31)                 |
	  ((((val & 0x03E0) * 255)/31) & 0x01FE0) << 3 |
		((((val & 0x7C00) * 255)/31) & 0x3FC00) << 6 );
}

inline uint16_t INT_8_16 (uint8_t val)
{
	return (val * 65535)/255;
}
