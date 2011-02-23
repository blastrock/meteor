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

#ifndef __PALETTE_WINDOW_H__
#define __PALETTE_WINDOW_H__

#include <gtkmm/window.h>
#include <gtkmm/drawingarea.h>

class PaletteWindow : public Gtk::Window
{
	public :
		PaletteWindow ();
		~PaletteWindow ();

	private :
		Gtk::DrawingArea m_bgPalette;
		Gtk::DrawingArea m_objPalette;
		Glib::RefPtr<Gdk::GC> m_bgGc;
		Glib::RefPtr<Gdk::GC> m_objGc;

		sigc::connection m_vblank_conn;

		void DrawGrid (Glib::RefPtr<Gdk::Window> area,
				Glib::RefPtr<Gdk::GC>& gc);

		bool on_bg_expose (GdkEventExpose* event);
		bool on_obj_expose (GdkEventExpose* event);
		void on_vblank ();
};

#endif
