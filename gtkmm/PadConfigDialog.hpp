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

#ifndef __PAD_CONFIG_DIALOG_H__
#define __PAD_CONFIG_DIALOG_H__

#include "Configurator.hpp"

#include <gtkmm/dialog.h>
#include <gtkmm/togglebutton.h>
#include <gtkmm/entry.h>

class PadConfigDialog : public Gtk::Dialog
{
	public:
		PadConfigDialog(Gtk::Window& parent, Configurator& conf);

	private:
		Configurator& m_conf;
		Configurator m_newConf;
		char m_binding;

		enum Buttons
		{
			BTN_A,
			BTN_B,
			BTN_L,
			BTN_R,
			BTN_START,
			BTN_SELECT,
			BTN_UP,
			BTN_DOWN,
			BTN_LEFT,
			BTN_RIGHT,
			NB_BTN
		};
		Gtk::ToggleButton m_buttons[NB_BTN];
		enum Entries
		{
			ETR_BATTERY,
			ETR_SSTATE,
			ETR_ROMS,
			NB_ETR
		};
		Gtk::Entry m_entries[NB_ETR];

		void on_show();
		void on_response(int resp);
		void on_bind(unsigned char btn);
		bool on_key_press_event(GdkEventKey* ev);
};

#endif
