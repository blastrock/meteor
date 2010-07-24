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

#ifndef __AMETEOR_H__
#define __AMETEOR_H__

#include "ameteor/memory.hpp"
#include "ameteor/io.hpp"
#include "ameteor/dma.hpp"
#include "ameteor/interpreter.hpp"
#include "ameteor/lcd.hpp"
#include "ameteor/clock.hpp"
#include "ameteor/timer.hpp"
#include "ameteor/sound.hpp"
#include "ameteor/keypad.hpp"
#include "ameteor/events.hpp"

namespace AMeteor
{
	extern Clock _clock;
	extern Io _io;
	extern Interpreter _cpu;
	extern Memory _memory;
	extern Dma _dma;
	extern Lcd _lcd;
	extern Sound _sound;
	extern Keypad _keypad;
	extern Timer _timer0;
	extern Timer _timer1;
	extern Timer _timer2;
	extern Timer _timer3;

	void Reset ();

	bool SaveState (const char* filename);
	bool LoadState (const char* filename);

	inline void Run ()
	{
		_cpu.Run();
	}

	inline void Stop ()
	{
		_cpu.Stop();
	}
}

#endif
