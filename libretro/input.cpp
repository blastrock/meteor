// Meteor - A Nintendo Gameboy Advance emulator
// Copyright (C) 2009-2011 Philippe Daouadi
// Copyright (C) 2011 Hans-Kristian Arntzen
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

#include "input.hpp"
#include "ameteor/core.hpp"
#include "libretro.h"

extern retro_input_state_t pretro_input;
extern retro_input_poll_t pretro_poll;
extern AMeteor::Core am_core;

void Input::InitAMeteor()
{
	am_core.get<AMeteor::Lcd>().sig_vblank.connect(
			syg::mem_fun(*this, &Input::CheckEvents));
}

void Input::CheckEvents()
{
	pretro_poll();

	uint16_t input = 0;

#define FETCH(key) \
	input |= pretro_input(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_##key) \
		? AMeteor::Keypad::BTN_##key : 0;
	FETCH(A);
	FETCH(B);
	FETCH(SELECT);
	FETCH(START);
	FETCH(RIGHT);
	FETCH(LEFT);
	FETCH(UP);
	FETCH(DOWN);
	FETCH(R);
	FETCH(L);
#undef FETCH

	input ^= 0x3FF;

	am_core.get<AMeteor::Keypad>().SetPadState(input);
}
