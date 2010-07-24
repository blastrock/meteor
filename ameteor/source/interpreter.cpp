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

#include "ameteor/interpreter.hpp"
#include "ameteor/cpu.hpp"
#include "ameteor/memory.hpp"
#include "ameteor/bios.hpp"
#include "globals.hpp"
#include "cpu_globals.hpp"
#include "ameteor.hpp"

#include "debug.hpp"
// XXX
unsigned long i;
bool strt = true;
namespace AMeteor
{
	Interpreter::Interpreter() :
		m_run(false),
		m_interrupt_(false),
		m_haltcnt(IO.GetRef8(Io::HALTCNT)),
		m_if(IO.GetRef16(Io::IF)),
		m_ie(IO.GetRef16(Io::IE))
	{
	}

	void Interpreter::SendInterrupt (uint16_t interrupt)
	{
		IO.GetRef16(Io::IF) |= interrupt;
		if ((interrupt & IO.DRead16(Io::IE)) &&
				(IO.DRead16(Io::IME) & 0x1) &&
				!m_st.icpsr.irq_d)
			// irq are enabled and theses irq are enabled...
			m_interrupt = true;
	}

	void Interpreter::CheckInterrupt ()
	{
		m_interrupt =
			(IO.DRead16(Io::IF) & IO.DRead16(Io::IE)) &&
			(IO.DRead16(Io::IME) & 0x1) &&
			!m_st.icpsr.irq_d;
	}

	//XXX
	void Interpreter::Run ()
	{
		m_run = true;
#if defined METDEBUG && defined METDEBUGLOG
		for (i = 0; i < fin && m_run; ++i)
		{
			// XXX
			if (strt && i > debut && m_haltcnt == 255)
			{
				debug("if : " << std::hex << m_if);
				debug("i : " << std::hex << std::uppercase << i);
			}
			if (!(i % 10000) && i < 0x13000)
			{
				//IO.GetRef16(Io::KEYINPUT) ^= 0x1; // toggle a
				//IO.GetRef16(Io::KEYINPUT) ^= 0x8; // toggle start
			}
#else
		while(m_run)
		{
#endif
			switch (m_haltcnt)
			{
				case 255: // normal mode
					if (FLAG_T)
					{
						if (R(15) & 0x1)
							_assert("PC not 16 bit aligned : " << IOS_ADD << R(15));

						code = MEM.Read16(R(15)-2);
						R(15) += 2;
						t_Code();
					}
					else
					{
						if (R(15) & 0x3)
							_assert("PC not 32 bit aligned : " << IOS_ADD << R(15));

						if (R(15) < 0x01000000 && !MEM.HasBios())
						{
							//XXX
							switch (R(15))
							{
								case 0x004:
									Bios::Bios000h();
									//XXX
									//goto arm;
									break;
								case 0x00C:
									Bios::Bios008h();
									break;
								case 0x01C:
									Bios::Bios018h();
									break;
								//case 0x134:
								case 0x254:
									Bios::Bios130h();
									break;
								//case 0x338:
								case 0x218:
									Bios::Bios338h();
									break;
								//case 0x16C:
								case 0x05C:
									Bios::Bios168h();
									break;
								default:
									_assert("Jump to " << IOS_ADD << R(15));
							}
						}
						else
						{
//XXX
arm:
							code = MEM.Read32(R(15)-4);
							R(15) += 4;
							a_Code();
						}
					}
					if (R(15) < 0x01000000 && FLAG_T && !MEM.HasBios())
						_assert("Jump to " << IOS_ADD << R(15));

					CLOCK.Commit();

					if (m_interrupt)
						// irq are enabled and there are irq waiting...
					{
						// FIXME : do we really need this ??
						// if not, we can get rid of save and load state and reset
						if (m_interrupt_)
						{
							m_interrupt_ = false;
							// XXX we must be sure the cpu isn't halted when an interrupt
							// occurs
							// should be removed after since it takes no time to make a new
							// iteration of the loop
							m_haltcnt = 255;
							CPU.Interrupt();
						}
						else
						{
							// XXX
							if (m_haltcnt != 255)
							{
								m_haltcnt = 255;
								CPU.Interrupt();
							}
							else
								m_interrupt_ = true;
						}
					}

					break;
				case 0: // halt mode
					if (m_if & m_ie) // interrupt occured
					{
						m_haltcnt = 255; // return to normal mode
						CPU.Interrupt();
						// XXX use an else
						break;
					}

					CLOCK.WaitForNext();

					// XXX remove this block
					if (m_if & m_ie) // interrupt occured
					{
						m_haltcnt = 255; // return to normal mode
						CPU.Interrupt();
					}

					break;
				case 1: // stop mode
					_assert("Stop mode not implemented");
					break;
				default:
					_assert("Unknown HALTCNT value : " << (int)m_haltcnt);
					break;
			}
		}
	}

#if 0
	void Interpreter::Run ()
	{
		m_run = true;
#if 1
		while (m_run)
#else
		for (i = 0; i < fin && m_run; ++i)
#endif
		{
			// XXX
			if (strt && i > debut && m_haltcnt == 255)
			{
				debug("if : " << m_if);
				debug("i : " << std::hex << std::uppercase << i);
			}
			if (!(i % 10000) && i < 0x13000)
			{
				//IO.GetRef16(Io::KEYINPUT) ^= 0x1; // toggle a
				//IO.GetRef16(Io::KEYINPUT) ^= 0x8; // toggle start
			}
			Step ();
		}
	}
#endif

	bool Interpreter::SaveState (gzFile file)
	{
		if (!gzwrite(file, &m_interrupt_, sizeof(m_interrupt_)))
			return false;

		return Cpu::SaveState(file);
	}

	bool Interpreter::LoadState (gzFile file)
	{
		if (gzread(file, &m_interrupt_, sizeof(m_interrupt_))
				!= sizeof(m_interrupt_))
			return false;

		return Cpu::LoadState(file);
	}
}
