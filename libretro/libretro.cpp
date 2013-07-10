// Meteor - A Nintendo Gameboy Advance emulator
// Copyright (C) 2009-2011 Philippe Daouadi
// Copyright (C) 2011 Hans-Kristian Arntzen
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


#include "libretro.h"
#include "video.hpp"
#include "audio.hpp"
#include "input.hpp"
#include "ameteor.hpp"
#include "ameteor/cartmem.hpp"
#include <sstream>
#include <stdio.h>
#include <cstring>
#include <assert.h>

Video am_video;
Audio am_audio;
Input am_input;

unsigned retro_api_version(void) { return RETRO_API_VERSION; }

retro_video_refresh_t pretro_refresh;
retro_audio_sample_t pretro_sample;
retro_input_poll_t pretro_poll;
retro_input_state_t pretro_input;
bool retro_rgb565;
static retro_environment_t pretro_environment;

void retro_set_video_refresh(retro_video_refresh_t video_refresh) { pretro_refresh = video_refresh; }
void retro_set_audio_sample(retro_audio_sample_t audio_sample) { pretro_sample = audio_sample; }
void retro_set_audio_sample_batch(retro_audio_sample_batch_t) {}
void retro_set_input_poll(retro_input_poll_t input_poll) { pretro_poll = input_poll; }
void retro_set_input_state(retro_input_state_t input_state) { pretro_input = input_state; }
void retro_set_environment(retro_environment_t environment) { pretro_environment = environment; }

void retro_set_controller_port_device(unsigned, unsigned) {}

void retro_get_system_info(struct retro_system_info *info)
{
	info->library_name = "Meteor GBA";
	info->library_version = "v1.4";
	info->need_fullpath = false;
	info->block_extract = false;
	info->valid_extensions = "gba";
}

void retro_get_system_av_info(struct retro_system_av_info *info)
{
	struct retro_game_geometry geom = { 240, 160, 240, 160 };
	struct retro_system_timing timing = { 16777216.0 / 280896.0, 44100.0 };

	info->geometry = geom;
	info->timing   = timing;
}

void retro_init(void) {}

void retro_reset(void) { AMeteor::Reset(AMeteor::UNIT_ALL ^ (AMeteor::UNIT_MEMORY_BIOS | AMeteor::UNIT_MEMORY_ROM)); }
void retro_deinit(void) { retro_reset(); }

static void init_first_run()
{
	static bool first_run = true;
	if (first_run)
	{
		AMeteor::_memory.LoadCartInferred();
		am_video.InitAMeteor();
		am_audio.InitAMeteor();
		am_input.InitAMeteor();
		first_run = false;
	}
}

void retro_run(void)
{
	init_first_run();
	pretro_poll();
	AMeteor::Run(10000000); // We emulate until VBlank.
}

size_t retro_serialize_size(void)
{
	init_first_run();
	std::ostringstream stream;
	AMeteor::SaveState(stream);
	unsigned serialize_size = stream.str().size();

	// We want constant sized streams, and thus we have to pad.
	uint32_t current_size = *(uint32_t*)(AMeteor::CartMemData + AMeteor::CartMem::MAX_SIZE);

	// Battery is not installed (yet!),
	// accomodate for the largest possible battery size if it does get installed after this check.
	// Flash 128kB + 4 byte state variable.
	if (!current_size)
		serialize_size += AMeteor::CartMem::MAX_SIZE + sizeof(uint32_t);

	return serialize_size;
}

bool retro_serialize(void *data, size_t size)
{
	std::ostringstream stream;
	AMeteor::SaveState(stream);

	std::string s = stream.str();
	if (s.size() > size)
		return false;

	std::memcpy(data, s.data(), s.size());
	return true;
}

bool retro_unserialize(const void *data, size_t size)
{
	std::istringstream stream;
	stream.str(std::string((char*)data, size));
	AMeteor::LoadState(stream);

	return true;
}

void retro_cheat_reset(void) {}
void retro_cheat_set(unsigned, bool, const char *) {}

bool retro_load_game(const struct retro_game_info *info)
{
	AMeteor::_memory.LoadRom((const uint8_t*)info->data, info->size);
   enum retro_pixel_format fmt = RETRO_PIXEL_FORMAT_RGB565;
   retro_rgb565 = pretro_environment(RETRO_ENVIRONMENT_SET_PIXEL_FORMAT, &fmt);
	return true;
}

bool retro_load_game_special(unsigned, const struct retro_game_info*, size_t) { return false; }

void retro_unload_game(void) {}

unsigned retro_get_region(void) { return RETRO_REGION_NTSC; }

void* retro_get_memory_data(unsigned id)
{
	if (id != RETRO_MEMORY_SAVE_RAM)
		return 0;

	return AMeteor::CartMemData;
}

size_t retro_get_memory_size(unsigned id)
{
	if (id != RETRO_MEMORY_SAVE_RAM)
		return 0;

	return AMeteor::CartMem::MAX_SIZE + 4;
}

