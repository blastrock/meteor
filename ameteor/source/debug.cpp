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

#include "debug.hpp"
#include "ameteor/cpu.hpp"
#include "globals.hpp"
#include "ameteor.hpp"

#include <sstream>
#include <map>

std::ofstream debug_stream("debug.log");
extern unsigned long i;

namespace AMeteor
{
	// TODO make this more guidelined (like the above assert)
	void debug_bits(uint32_t u)
	{
		for (register int8_t c = 31; c >= 0; --c)
		{
			STDBG << !!(u & (((uint32_t)0x1) << c));
			if (!(c % 8))
				STDBG << ' ';
		}
		STDBG << std::endl;
	}

	void debug_bits_16(uint16_t u)
	{
		for (register int8_t c = 15; c >= 0; --c)
		{
			STDBG << !!(u & (((uint32_t)0x1) << c));
			if (!(c % 8))
				STDBG << ' ';
		}
		STDBG << std::endl;
	}

#ifdef MYDEBUG
	void PrintRegs ()
	{
#if 1
		for (uint8_t c = 0; c <= 15; ++c)
			if (R(c))
#else
		const int c = 15;
#endif
				STDBG << "R" << std::setbase(10) << (int)c << " = " << IOS_ADD << R(c) << '\n';
#if 1
		CPU.UpdateCpsr();
		if (CPSR)
			STDBG << "R16 = " << IOS_ADD << CPSR << '\n';
#endif
		if (0&&SPSR)
			STDBG << "R17 = " << IOS_ADD << SPSR << '\n';
		STDBG << "IRQ R13 = " << IOS_ADD << CPU.irq13() << '\n';
		STDBG << "IRQ R14 = " << IOS_ADD << CPU.irq14() << '\n';
		STDBG << "SVC R13 = " << IOS_ADD << CPU.svc13() << '\n';
		STDBG << "SVC R14 = " << IOS_ADD << CPU.svc14() << '\n';
		STDBG << std::flush;
	}

	void PrintStack (uint32_t stackadd)
	{
		if (i < debut)return;
		uint32_t add = stackadd;
		debug("Stack : " << IOS_ADD << add);
		for (; add < 0x03008000; add += 4)
			debug(IOS_ADD << MEM.Read32(add));
	}
#endif
}
