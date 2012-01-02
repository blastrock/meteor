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

#include "mym/audio.hpp"
#include <cstring>
#include <ameteor.hpp>

namespace mym
{
	Audio::Audio() :
		m_device(0)
	{
		ao_initialize();
	}

	Audio::~Audio ()
	{
		Uninit();
	}

	void Audio::InitAMeteor()
	{
		AMeteor::_sound.GetSpeaker().SetFrameSlot(
				syg::mem_fun(*this, &Audio::PlayFrames));
	}

	void Audio::Init()
	{
		ao_sample_format format;
		std::memset(&format, 0, sizeof(format));
		format.bits = 16;
		format.channels = 2;
		format.rate = 44100;
		format.byte_format = AO_FMT_NATIVE;

		m_device = ao_open_live(ao_default_driver_id(), &format, NULL);
		if (m_device == NULL)
		{
			puts("Cannot open sound device");
			abort();
		}
	}

	void Audio::Uninit()
	{
		if (m_device)
			ao_close(m_device);
	}

	void Audio::PlayFrames(const int16_t* data)
	{
		// ugly :(
		ao_play(m_device, const_cast<char*>((const char*)data), 2*2);
	}
}
