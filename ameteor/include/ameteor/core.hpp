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

#ifndef __AMETEOR_H__
#define __AMETEOR_H__

#include <ameteor/bios.hpp>
#include <ameteor/clock.hpp>
#include <ameteor/dma.hpp>
#include <ameteor/interpreter.hpp>
#include <ameteor/io.hpp>
#include <ameteor/keypad.hpp>
#include <ameteor/lcd.hpp>
#include <ameteor/memory.hpp>
#include <ameteor/sound.hpp>
#include <ameteor/timer.hpp>

namespace AMeteor
{
const uint32_t UNIT_CLOCK = 0x0001;
const uint32_t UNIT_IO = 0x0002;
const uint32_t UNIT_CPU = 0x0004;
const uint32_t UNIT_MEMORY = 0x0008;
const uint32_t UNIT_DMA = 0x0010;
const uint32_t UNIT_LCD = 0x0020;
const uint32_t UNIT_SOUND = 0x0040;
const uint32_t UNIT_KEYPAD = 0x0080;
const uint32_t UNIT_TIMER0 = 0x0100;
const uint32_t UNIT_TIMER1 = 0x0200;
const uint32_t UNIT_TIMER2 = 0x0400;
const uint32_t UNIT_TIMER3 = 0x0800;
const uint32_t UNIT_MEMORY_ROM = 0x1000;
const uint32_t UNIT_MEMORY_BIOS = 0x2000;
const uint32_t UNIT_ALL = 0x3FFF;

class Core
  // the clock must be initialized first since there are devices like lcd which
  // needs to set the timer
  : private Clock
  , private Io
  // the interpreter (which is in the cpu) takes io addresses, thus the cpu must
  // be initialized after io
  , private Interpreter
  , private Memory
  , private Dma
  // the lcd must be initialized after the memory since it takes pointers from
  // it
  , private Lcd
  // the sound must be initialized after the io since it takes references from
  // it
  , private Sound
  // the keypad needs to take the vblank event from lcd, so it must be
  // initialized after lcd it must also be initialized after io since it takes
  // the keyinput reference
  , private Keypad
  , private Timer<3>
  , private Timer<2>
  , private Timer<1>
  , private Timer<0>
  , private Bios
{
public:
  Core();

  template <typename Target>
  auto& get()
  {
    return *static_cast<Target*>(this);
  }

  template <typename Target, typename Source>
  static Target& get(Source& src)
  {
    return static_cast<Core*>(&src)->get<Target>();
  }

  void Reset(uint32_t units);

  bool SaveState(std::ostream& stream);
  bool LoadState(std::istream& stream);

  inline void Run(unsigned int cycles)
  {
    get<Interpreter>().Run(cycles);
  }

  inline void Stop()
  {
    get<Interpreter>().Stop();
  }
};

template <>
inline auto& Core::get<Cpu>()
{
  return *static_cast<Interpreter*>(this);
}
}

#endif
