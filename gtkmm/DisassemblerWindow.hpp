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

#ifndef __DISASSEMBLER_WINDOW_H__
#define __DISASSEMBLER_WINDOW_H__

#include "DisassemblerTreeView.hpp"

#include <gtkmm/window.h>
#include <gtkmm/entry.h>
#include <gtkmm/spinbutton.h>

class DisassemblerWindow : public Gtk::Window
{
	public :
		DisassemblerWindow ();

		void Reload ()
		{
			m_treeView.Reload();
		}

	private :
		Gtk::Entry m_entryOffset;
		Gtk::SpinButton m_spinNumInstr;
		DisassemblerTreeView m_treeView;

		void on_num_instr_changed ();
		void on_go_to ();
		void on_instruction_set_changed ();
};

#endif
