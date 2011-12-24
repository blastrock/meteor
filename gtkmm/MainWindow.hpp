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

#ifndef __MAIN_WINDOW_H__
#define __MAIN_WINDOW_H__

#include "DisassemblerWindow.hpp"
#include "PaletteWindow.hpp"
#include "VramWindow.hpp"
#include "AboutDialog.hpp"
#include "PadConfigDialog.hpp"

#include <mym/window.hpp>
#include <mym/audio.hpp>
#include <mym/events.hpp>

#include <gtkmm/window.h>
#include <gtkmm/checkmenuitem.h>
#include <gtkmm/statusbar.h>
#include <gtkmm/drawingarea.h>
#include <gtkmm/box.h>

class MainWindow : public Gtk::Window
{
	public :
		MainWindow ();

	private :
		mym::Window m_window;
		mym::Audio m_audio;
		mym::Events m_events;

		Configurator m_config;

		Gtk::VBox m_mainvbox;
		Gtk::Statusbar m_statusbar;
		Gtk::DrawingArea m_viewport;
		Gtk::Image m_mainimage;

		DisassemblerWindow m_disassemblerWindow;
		PaletteWindow m_paletteWindow;
		VramWindow m_vramWindow;
		AboutDialog m_aboutDialog;
		PadConfigDialog m_padConfigDialog;

		Glib::RefPtr<Gtk::CheckMenuItem> m_refDisassemblerCheck;
		Glib::RefPtr<Gtk::CheckMenuItem> m_refPaletteCheck;
		Glib::RefPtr<Gtk::CheckMenuItem> m_refVramCheck;

		std::string m_sstatePath, m_batteryPath, m_romPath;
		std::string m_openFile;

		bool m_running;

		void on_open ();
		void on_open_bios ();
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
		void on_config_pad();
		void on_close ();
		void on_close_bios ();

		void on_save_state (uint8_t n);
		void on_load_state (uint8_t n);

		void on_quit ();
		bool on_delete_event(GdkEventAny* event);

		bool on_key_press_event(GdkEventKey* key);
		bool on_key_release_event(GdkEventKey* key);
};

#endif
