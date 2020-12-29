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

#include "ameteor/interpreter.hpp"
#include "ameteor/bios.hpp"
#include "ameteor/core.hpp"
#include "ameteor/cpu.hpp"
#include "ameteor/disassembler/instruction.hpp"
#include "ameteor/memory.hpp"
#include "globals.hpp"

#include "cpu_globals.hpp"

#include "debug.hpp"

namespace AMeteor
{
Interpreter::Interpreter()
  : m_run(false)
  , m_interrupt_(false)
  , m_haltcnt(IO.GetRef8(Io::HALTCNT))
  , m_if(IO.GetRef16(Io::IF))
  , m_ie(IO.GetRef16(Io::IE))
{
}

void Interpreter::Run(unsigned int cycles)
{
  m_run = true;
  CLOCK.ResetCounter();
  while (m_run && CLOCK.GetCounter() < cycles)
  {
    switch (m_haltcnt)
    {
    case 255: // normal mode
      if (FLAG_T)
      {
        while (!CLOCK.IsEventPending())
        {
#if METEOR_TRACE
          std::cout << CLOCK.GetCounter() << std::endl;
#endif
          PrintRegs(*this);
          if (R(15) & 0x1)
            met_abort("PC not 16 bit aligned : " << IOS_ADD << R(15));

          code = MEM.ReadQuick16(R(15) - 2);
          TRACE_THUMB_INSTRUCTION(R(15), code);
          R(15) += 2;
          t_Code();
        }
      }
      else
      {
        while (!CLOCK.IsEventPending())
        {
#if METEOR_TRACE
          std::cout << CLOCK.GetCounter() << std::endl;
#endif
          PrintRegs(*this);
          if (R(15) & 0x3)
            met_abort("PC not 32 bit aligned : " << IOS_ADD << R(15));

          if (R(15) < 0x01000000 && !MEM.HasBios())
          {
            switch (R(15))
            {
            case 0x004:
              BIOS.Bios000h();
              break;
            case 0x00C:
              BIOS.Bios008h();
              break;
            case 0x01C:
              BIOS.Bios018h();
              break;
            case 0x134:
              BIOS.Bios130h();
              break;
            case 0x33C:
              BIOS.Bios338h();
              break;
            case 0x16C:
              BIOS.Bios168h();
              break;
            default:
              met_abort("Jump to " << IOS_ADD << R(15));
            }
            CLOCK.SetEventPending();
          }
          else
          {
            code = MEM.ReadQuick32(R(15) - 4);
            TRACE_ARM_INSTRUCTION(R(15), code);
            R(15) += 4;
            a_Code();
          }
        }
      }
      if (R(15) < 0x01000000 && FLAG_T && !MEM.HasBios())
        met_abort("Jump to " << IOS_ADD << R(15));

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
          {
            m_interrupt_ = true;
            CLOCK.SetEventPending(1);
          }
        }
      }

      break;
    case 0:            // halt mode
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
      met_abort("Stop mode not implemented");
      break;
    default:
      met_abort("Unknown HALTCNT value : " << (int)m_haltcnt);
      break;
    }
  }
  m_run = false;
}

bool Interpreter::SaveState(std::ostream& stream)
{
  SS_WRITE_VAR(m_interrupt_);

  return Cpu::SaveState(stream);
}

bool Interpreter::LoadState(std::istream& stream)
{
  SS_READ_VAR(m_interrupt_);

  return Cpu::LoadState(stream);
}
}
