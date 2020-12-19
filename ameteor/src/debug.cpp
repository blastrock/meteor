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

#include "debug.hpp"
#include "ameteor/core.hpp"
#include "ameteor/cpu.hpp"
#include "globals.hpp"

#include <map>
#include <sstream>

namespace AMeteor
{
// TODO make this more guidelined (like the above assert)
void debug_bits(uint32_t u)
{
#ifndef NDEBUG
  for (int8_t c = 31; c >= 0; --c)
  {
    STDBG << !!(u & (((uint32_t)0x1) << c));
    if (!(c % 8))
      STDBG << ' ';
  }
  STDBG << std::endl;
#else
  (void)u;
#endif
}

void debug_bits_16(uint16_t u)
{
#ifndef NDEBUG
  for (int8_t c = 15; c >= 0; --c)
  {
    STDBG << !!(u & (((uint32_t)0x1) << c));
    if (!(c % 8))
      STDBG << ' ';
  }
  STDBG << std::endl;
#else
  (void)u;
#endif
}

#ifdef METEOR_TRACE
void PrintRegs(Cpu& cpu)
{
  static uint32_t regs[17] = {0};

  for (uint8_t c = 0; c <= 15; ++c)
    if (cpu.Reg(c) != regs[c])
    {
      STDBG << "R" << std::setbase(10) << (int)c << " = " << IOS_ADD
            << cpu.Reg(c) << '\n';
      regs[c] = cpu.Reg(c);
    }
  cpu.UpdateCpsr();
  if (cpu.Reg(16) != regs[16])
  {
    STDBG << "R16 = " << IOS_ADD << cpu.Reg(16) << '\n';
    regs[16] = cpu.Reg(16);
  }
}

void PrintStack(Memory& memory, uint32_t stackadd)
{
  uint32_t add = stackadd;
  debug("Stack : " << IOS_ADD << add);
  for (; add < 0x03008000; add += 4)
    debug(IOS_ADD << memory.Read32(add));
}
#endif
}
