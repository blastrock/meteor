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

#ifndef __DISASSEMBLER_TREE_VIEW_H__
#define __DISASSEMBLER_TREE_VIEW_H__

#include <gtkmm/box.h>
#include <gtkmm/treeview.h>
#include <gtkmm/liststore.h>
#include <gtkmm/scrollbar.h>
#include <gtkmm/treemodelcolumn.h>

class DisassemblerTreeView : public Gtk::HBox
{
	public :
		static const uint8_t SET_THUMB = 2;
		static const uint8_t SET_ARM = 4;

		DisassemblerTreeView ();

		void SetNumInstr (uint8_t numinstr);
		void GoTo (uint32_t to);
		void Reload ();

		void SwitchInstructionSet ();

	private :
		Gtk::TreeView m_treeView;
		Glib::RefPtr<Gtk::ListStore> m_refListStore;
		Gtk::VScrollbar m_scrollbar;
		class DisassemblerColumns : public Gtk::TreeModelColumnRecord
		{
			public :
				DisassemblerColumns ()
					{ add(offset); add(op); add(args); }

				Gtk::TreeModelColumn<uint32_t> offset;
				Gtk::TreeModelColumn<Glib::ustring> op;
				Gtk::TreeModelColumn<Glib::ustring> args;
		} m_columns;

		uint32_t m_lastPos;
		uint8_t m_numRows;
		uint8_t m_instSize;

		void on_scroll ();
		bool on_scroll_event (GdkEventScroll* event);
		bool on_key_press (GdkEventKey* event);

		void FillRow (const Gtk::TreeRow& row);
};

#endif
