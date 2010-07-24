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

#ifndef __VRAM_AREA_H__
#define __VRAM_AREA_H__

#include <gtkmm/drawingarea.h>

class VramArea : public Gtk::DrawingArea
{
	public :
		VramArea();

		void Draw (uint32_t charBase, int8_t palette);

	private :
		Glib::RefPtr<Gdk::GC> m_gc;
};

#endif
