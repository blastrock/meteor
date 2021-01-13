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

#ifndef __DEBUG_H__
#define __DEBUG_H__

#include <ameteor/core.hpp>

#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>

#if METEOR_ASSERTIONS
#define met_abort(str)                                                     \
  {                                                                        \
    std::cerr << IOS_NOR << "Fatal error :\n"                              \
              << str << "\nFile : " << __FILE__ << "\nLine : " << __LINE__ \
              << "\nr15 = " << IOS_ADD << CPU.Reg(15)                      \
              << "\n[r15] = " << IOS_ADD << MEM.Read32(CPU.Reg(15))        \
              << "\nFlag T : " << CPU.ICpsr().thumb << std::endl;          \
    abort();                                                               \
  }
#define met_abort_raw(str)                                                 \
  {                                                                        \
    std::cerr << IOS_NOR << "Fatal error :\n"                              \
              << str << "\nFile : " << __FILE__ << "\nLine : " << __LINE__ \
              << std::endl;                                                \
    abort();                                                               \
  }
#else
#define met_abort_raw(str) \
  {                        \
  }
#define met_abort(str) \
  {                    \
  }
#endif

#define STDBG std::cout
//#define STDBG debug_stream

#ifndef NDEBUG
#define debug(str) STDBG << str << std::endl
#define debug_(str) STDBG << str
#else
#define debug(s) \
  {              \
  }
#define debug_(s) \
  {               \
  }
#endif

#define IOS_ADD                                                 \
  "0x" << std::setbase(16) << std::setw(8) << std::setfill('0') \
       << std::uppercase
#define IOS_NOR std::setbase(10) << std::setw(0) << std::setfill(' ')

namespace AMeteor
{
void debug_bits(uint32_t u);
void debug_bits_16(uint16_t u);

#if METEOR_TRACE
void PrintRegs(Cpu& cpu);
void PrintStack(Memory& memory, uint32_t stackadd);
#define TRACE_THUMB_INSTRUCTION(pc, code)                               \
  std::cerr << IOS_ADD << pc << ' '                                     \
            << Disassembler::Instruction(pc, (uint16_t)code).ToString() \
            << std::endl;
#define TRACE_ARM_INSTRUCTION(pc, code)                                 \
  std::cerr << IOS_ADD << pc << ' '                                     \
            << Disassembler::Instruction(pc, (uint32_t)code).ToString() \
            << std::endl;
#else
#define PrintRegs(c)
#define PrintStack(m, b)
#define TRACE_THUMB_INSTRUCTION(pc, code)
#define TRACE_ARM_INSTRUCTION(pc, code)
#endif
}

#endif
