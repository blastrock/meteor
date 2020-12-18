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

#include "ameteor/core.hpp"
#include "debug.hpp"
#include "globals.hpp"
#include <cstring>
#include <sstream>

// TODO add version
#define SS_MAGIC_STRING ("AMeteor SaveState")
#define SS_MS_SIZE (sizeof(SS_MAGIC_STRING) - 1)

namespace AMeteor
{
namespace
{
class AMeteor
{
public:
  AMeteor()
  {
    Audio::InitNoise();
  }
} __ameteor;
}

Core::Core()
{
}

void Core::Reset(uint32_t units)
{
#define RESET(u, e)     \
  if (units & UNIT_##e) \
    get<u>().Reset();
  RESET(Clock, CLOCK);
  RESET(Io, IO);
  RESET(Cpu, CPU);
  RESET(Dma, DMA);
  RESET(Lcd, LCD);
  RESET(Sound, SOUND);
  RESET(Keypad, KEYPAD);
  RESET(Timer<0>, TIMER0);
  RESET(Timer<1>, TIMER1);
  RESET(Timer<2>, TIMER2);
  RESET(Timer<3>, TIMER3);
  // RESET(Bios, BIOS);
#undef RESET
  if (units & UNIT_MEMORY)
    get<Memory>().Reset(units);
}

bool Core::SaveState(std::ostream& stream)
{
  if (get<Cpu>().IsRunning())
    return false;

  SS_WRITE_DATA(SS_MAGIC_STRING, SS_MS_SIZE);

#define SAVE(dev)                    \
  if (!get<dev>().SaveState(stream)) \
  return false
  SAVE(Clock);
  SAVE(Io);
  SAVE(Cpu);
  SAVE(Memory);
  SAVE(Dma);
  SAVE(Lcd);
  SAVE(Sound);
  // SAVE(Keypad);
  SAVE(Timer<0>);
  SAVE(Timer<1>);
  SAVE(Timer<2>);
  SAVE(Timer<3>);
  // SAVE(Bios);
#undef SAVE

  return true;
}

bool Core::LoadState(std::istream& stream)
{
  if (get<Cpu>().IsRunning())
    return false;

  {
    char buf[SS_MS_SIZE];
    SS_READ_DATA(buf, SS_MS_SIZE);
    if (std::memcmp(buf, SS_MAGIC_STRING, SS_MS_SIZE))
      return false;
  }

#define LOAD(dev)                    \
  if (!get<dev>().LoadState(stream)) \
  return false
  LOAD(Clock);
  LOAD(Io);
  LOAD(Cpu);
  LOAD(Memory);
  LOAD(Dma);
  LOAD(Lcd);
  LOAD(Sound);
  // LOAD(Keypad);
  LOAD(Timer<0>);
  LOAD(Timer<1>);
  LOAD(Timer<2>);
  LOAD(Timer<3>);
  // LOAD(Bios);
#undef LOAD

  uint8_t xxx;
  // if there is garbage at end of file
  if (stream.read((char*)&xxx, 1))
    return false;

  return true;
}
}
