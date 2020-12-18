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

#include "audio.hpp"
#include "libretro.h"
#include "ameteor/core.hpp"
#include <assert.h>

extern retro_audio_sample_t pretro_sample;
extern AMeteor::Core am_core;

void Audio::InitAMeteor()
{
	am_core.get<AMeteor::Sound>().GetSpeaker().SetFrameSlot(
			syg::mem_fun(*this, &Audio::PlayFrames));
}

void Audio::PlayFrames(const int16_t* data)
{
	pretro_sample(data[0], data[1]);
}
