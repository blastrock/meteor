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

#include "ameteor.hpp"
#include "gzhelper.hpp"
#include "graphics/filters/hq4x.hpp"
#include "debug.hpp"
#include <cstring>

#include <ao/ao.h>

#define SS_MAGIC_STRING "AMeteor SaveState"
#define SS_MS_SIZE sizeof(SS_MAGIC_STRING)

namespace AMeteor
{
	namespace
	{
		class AMeteor
		{
			public :
				AMeteor ()
				{
					ao_initialize();
					// FIXME XXX not cool
					if (!Graphics::Filters::InitLUTs())
						met_abort("MMX not supported");
					Audio::InitNoise();
				}

				~AMeteor ()
				{
					ao_shutdown();
				}
		} __ameteor;
	}

	// the clock must be initialized first since there are devices like
	// lcd which needs to set the timer
	Clock _clock;
	Io _io;
	// the interpreter (which is in the cpu) takes io addresses, thus the
	// cpu must be initialized after io
	Interpreter _cpu;
	Memory _memory;
	Dma _dma;
	// the lcd must be initialized after the memory since it takes
	// pointers from it
	Lcd _lcd;
	// the sound must be initialized after the io since it takes references
	// from it
	Sound _sound;
	// the keypad needs to take the vblank event from lcd, so it must be
	// initialized after lcd
	// it must also be initialized after io since it takes the keyinput
	// reference
	Keypad _keypad;
	Timer _timer3(3, NULL);
	Timer _timer2(2, &_timer3);
	Timer _timer1(1, &_timer2);
	Timer _timer0(0, &_timer1);

	void Reset ()
	{
		_clock.Reset();
		_io.Reset();
		_cpu.Reset();
		_memory.Reset();
		_dma.Reset();
		_lcd.Reset();
		_sound.Reset();
		_keypad.Reset();
		_timer0.Reset();
		_timer1.Reset();
		_timer2.Reset();
		_timer3.Reset();
	}

	bool SaveState (const char* filename)
	{
		if (_cpu.IsRunning())
			return false;

		GzHelper file(gzopen(filename, "wb"));
		if (!file)
			return false;
		if (!gzwrite(file, SS_MAGIC_STRING, sizeof(SS_MAGIC_STRING)-1))
			return false;

#define SAVE(dev) \
	if (!dev.SaveState(file)) \
		return false
		SAVE(_clock);
		SAVE(_io);
		SAVE(_cpu);
		SAVE(_memory);
		SAVE(_dma);
		SAVE(_lcd);
		SAVE(_sound);
		//SAVE(_keypad);
		SAVE(_timer0);
		SAVE(_timer1);
		SAVE(_timer2);
		SAVE(_timer3);
#undef SAVE

		return true;
	}

	bool LoadState (const char* filename)
	{
		if (_cpu.IsRunning())
			return false;

		GzHelper file(gzopen(filename, "rb"));
		if (!file)
			return false;

		{
			char buf[SS_MS_SIZE-1];
			if (gzread(file, buf, SS_MS_SIZE-1) != SS_MS_SIZE-1)
				return false;
			if (std::strncmp(buf, SS_MAGIC_STRING, SS_MS_SIZE-1))
				return false;
		}

#define LOAD(dev) \
	if (!dev.LoadState(file)) \
		return false
		LOAD(_clock);
		LOAD(_io);
		LOAD(_cpu);
		LOAD(_memory);
		LOAD(_dma);
		LOAD(_lcd);
		LOAD(_sound);
		//LOAD(_keypad);
		LOAD(_timer0);
		LOAD(_timer1);
		LOAD(_timer2);
		LOAD(_timer3);
#undef LOAD

		uint8_t xxx;
		// if there is garbage at end of file
		if (gzread(file, &xxx, 1) != 0)
			return false;

		return true;
	}
}
