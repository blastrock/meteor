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

#ifndef __VRAM_WINDOW_H__
#define __VRAM_WINDOW_H__

#include <gtkmm/window.h>
#include <gtkmm/radiobutton.h>
#include <gtkmm/spinbutton.h>

#include "VramArea.hpp"

class VramWindow : public Gtk::Window
{
	public :
		VramWindow ();
		~VramWindow ();

	private :
		Gtk::RadioButton m_col16, m_col32;
		Gtk::RadioButton m_base0, m_base4, m_base8, m_baseC, m_baseObj;
		Gtk::SpinButton m_palette;
		VramArea m_area;

		sigc::connection m_vblank_conn;

		void on_radio_changed (Gtk::RadioButton* radio);
		void on_refresh ();
		void on_vblank ();
};

#endif
