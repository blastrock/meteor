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

#include "VramArea.hpp"
#include "globals.hpp"
#include <gtkmm/container.h>
#include <cstring>

#include "ameteor.hpp"

VramArea::VramArea () :
	Gtk::DrawingArea()
{
}

void VramArea::Draw (uint32_t charBase, int8_t palette)
{
	uint8_t* pBaseChar = AMeteor::_memory.GetRealAddress(charBase),
		*pTLine, *pTile, *pLine, *pPixel;
	uint16_t* pPalette = (uint16_t*)AMeteor::_memory.GetRealAddress(0x05000000);
	uint32_t* out = new uint32_t[256*256], *poTLine, *poTile, *poLine, *poPixel;
	std::memset(out, 0, 256*256*4);

	if (charBase == 0x06010000)
		pPalette += 256;

	if (palette == -1)
	{
		// for each line of tiles
		for (pTLine = pBaseChar, poTLine = out; pTLine - pBaseChar < 0x8000;
				pTLine += 32*8*8, poTLine += 32*8*8)
			// for each tile of the line
			for (pTile = pTLine, poTile = poTLine; pTile - pTLine < 8*8*32;
					pTile += 8*8, poTile += 8)
				// for each line of the tile
				for (pLine = pTile, poLine = poTile; pLine - pTile < 8*8;
						pLine += 8, poLine += 256)
					// for each pixel of the line
					for (pPixel = pLine, poPixel = poLine; pPixel - pLine < 8;
							++pPixel, ++poPixel)
						*poPixel = COLORS_16_32(pPalette[*pPixel]);
	}
	else
	{
		pPalette += palette*16;
		// for each line of tiles
		for (pTLine = pBaseChar, poTLine = out; pTLine - pBaseChar < 0x8000;
				pTLine += 32*8*8/2, poTLine += 32*8*8)
			// for each tile of the line
			for (pTile = pTLine, poTile = poTLine; pTile - pTLine < 8*8*32/2;
					pTile += 8*8/2, poTile += 8)
				// for each line of the tile
				for (pLine = pTile, poLine = poTile; pLine - pTile < 8*8/2;
						pLine += 8/2, poLine += 256)
					// for each pixel of the line
					for (pPixel = pLine, poPixel = poLine; pPixel - pLine < 8/2; )
					{
						*poPixel++ = COLORS_16_32(pPalette[*pPixel & 0x0F]);
						*poPixel++ = COLORS_16_32(pPalette[*pPixel >> 4]);
						++pPixel;
					}
	}

	if (!m_gc)
		m_gc = Gdk::GC::create(this->get_window());

	this->get_window()->draw_rgb_32_image(m_gc, 0, 0, 256, 256,
			Gdk::RGB_DITHER_NONE, (guchar*)out, 256*4);

	delete [] out;
}
