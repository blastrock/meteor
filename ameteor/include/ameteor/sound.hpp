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

#ifndef __SOUND_H__
#define __SOUND_H__

#include "audio/speaker.hpp"
#include "clock.hpp"
#include <stdint.h>
#include <zlib.h>

namespace AMeteor
{
	class Sound
	{
		public :
			Sound ();

			inline void SetSampleskip (uint16_t skip);

			void Reset ();

			void UpdateCntH1 (uint8_t val);

			inline void ResetSound1 ();
			inline void ResetSound2 ();
			inline void ResetSound4 ();

			inline void ResetSound1Envelope ();
			inline void ResetSound2Envelope ();
			inline void ResetSound4Envelope ();

			void TimerOverflow (uint8_t timernum);

			inline void SendDigitalA (uint8_t* buffer);
			inline void SendDigitalB (uint8_t* buffer);

			bool SaveState (gzFile file);
			bool LoadState (gzFile file);

		private :
			Audio::Speaker m_speaker;

			uint8_t m_fATimer, m_fBTimer;

			inline void TimerOverflowA ();
			inline void TimerOverflowB ();

			void TimeEvent ()
			{
				m_speaker.SoundTick();
			}

			friend void Clock::Commit ();
	};

	inline void Sound::SetSampleskip (uint16_t skip)
	{
		m_speaker.SetSampleskip(skip);
	}

	inline void Sound::ResetSound1 ()
	{
		m_speaker.ResetSound1();
	}

	inline void Sound::ResetSound2 ()
	{
		m_speaker.ResetSound2();
	}

	inline void Sound::ResetSound4 ()
	{
		m_speaker.ResetSound4();
	}

	inline void Sound::ResetSound1Envelope ()
	{
		m_speaker.ResetSound1Envelope();
	}

	inline void Sound::ResetSound2Envelope ()
	{
		m_speaker.ResetSound2Envelope();
	}

	inline void Sound::ResetSound4Envelope ()
	{
		m_speaker.ResetSound4Envelope();
	}

	inline void Sound::SendDigitalA (uint8_t* buffer)
	{
		m_speaker.FillFifoA((int8_t*)buffer);
	}

	inline void Sound::SendDigitalB (uint8_t* buffer)
	{
		m_speaker.FillFifoB((int8_t*)buffer);
	}
}

#endif
