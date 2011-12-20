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
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.

#include "ameteor/graphics/renderer.hpp"
#include "ameteor.hpp"
#include "libsnes.hpp"
#include <stdio.h>

extern snes_video_refresh_t psnes_refresh;

namespace AMeteor
{
	namespace Graphics
	{
		Renderer::Renderer (const uint16_t* surface) :
			m_base(surface)
		{}

		void Renderer::VBlank ()
		{
			uint16_t buf[240 * 160];
			for (unsigned i = 0; i < 240 * 160; i++)
			{
				uint16_t col = m_base[i];
				uint16_t b = (col >> 10) & 0x1f;
				uint16_t g = (col >> 5) & 0x1f;
				uint16_t r = (col >> 0) & 0x1f;
				buf[i] = (r << 10) | (g << 5) | (b << 0);
			}

			// FIXME should not be needed
			AMeteor::Stop();
			psnes_refresh(buf, 240, 160);
		}
	}
}
