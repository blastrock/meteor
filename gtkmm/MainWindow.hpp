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

#ifndef __MAIN_WINDOW_H__
#define __MAIN_WINDOW_H__

#include "DisassemblerWindow.hpp"
#include "PaletteWindow.hpp"
#include "VramWindow.hpp"

#include <gtkmm/window.h>
#include <gtkmm/checkmenuitem.h>
#include <gtkmm/statusbar.h>

class MainWindow : public Gtk::Window
{
	public :
		MainWindow ();

	private :
		Gtk::Statusbar m_statusbar;

		DisassemblerWindow m_disassemblerWindow;
		PaletteWindow m_paletteWindow;
		VramWindow m_vramWindow;

		Glib::RefPtr<Gtk::CheckMenuItem> m_refDisassemblerCheck;
		Glib::RefPtr<Gtk::CheckMenuItem> m_refPaletteCheck;
		Glib::RefPtr<Gtk::CheckMenuItem> m_refVramCheck;

		void on_open ();
		void on_menu_disassembler_toggle ();
		void on_menu_palette_toggle ();
		void on_menu_vram_toggle ();
		void on_menu_about ();
		void on_disassembler_hide ();
		void on_palette_hide ();
		void on_vram_hide ();
		void on_run ();
		void on_stop ();
		void on_reset ();

		void on_save_state ();
		void on_load_state ();

		void on_quit ();
		bool on_delete_event(GdkEventAny* event);

		void on_vblank ();
};

#endif
