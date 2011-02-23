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

#include "DisassemblerWindow.hpp"

#include <gtkmm/menu.h>
#include <gtkmm/menubar.h>
#include <gtkmm/box.h>
#include <gtkmm/toolitem.h>
#include <gtkmm/separatortoolitem.h>
#include <gtkmm/toolbar.h>
#include <gtkmm/label.h>
#include <gtkmm/stock.h>

#include "debug.hpp"

DisassemblerWindow::DisassemblerWindow () :
	Gtk::Window(),
	m_treeView()
{
	this->set_title("Meteor - Disassembler");
	this->set_default_size(500, 10);

	Gtk::RadioButtonGroup rgroup_iset;

	Gtk::Menu* menu_file = manage(new Gtk::Menu);
	menu_file->items().push_back(Gtk::Menu_Helpers::RadioMenuElem(rgroup_iset,
				"ARM", sigc::mem_fun(*this,
					&DisassemblerWindow::on_instruction_set_changed)));
	menu_file->items().push_back(Gtk::Menu_Helpers::RadioMenuElem(rgroup_iset,
				"THUMB"));
	menu_file->items().push_back(Gtk::Menu_Helpers::SeparatorElem());
	menu_file->items().push_back(Gtk::Menu_Helpers::StockMenuElem(
				Gtk::Stock::CLOSE, sigc::mem_fun(*this,
					&DisassemblerWindow::hide)));

	Gtk::MenuBar* menubar = manage(new Gtk::MenuBar);
	menubar->items().push_back(Gtk::Menu_Helpers::MenuElem("_Disassembler",
				*menu_file));

	m_spinNumInstr.set_range(10, 100);
	m_spinNumInstr.set_increments(1, 4);
	m_spinNumInstr.set_value(20);
	m_treeView.SetNumInstr(20);

	m_entryOffset.set_max_length(8);
	m_entryOffset.set_text("08000000");

	Gtk::ToolItem* item_offlabel = manage(new Gtk::ToolItem);
	item_offlabel->add(*manage(new Gtk::Label("Go to offset : ")));

	Gtk::ToolItem* item_offentry = manage(new Gtk::ToolItem);
	item_offentry->add(m_entryOffset);

	Gtk::ToolButton* item_gooff = manage(new Gtk::ToolButton(Gtk::Stock::OK));

	Gtk::SeparatorToolItem* tool_separator = manage(new Gtk::SeparatorToolItem());

	Gtk::ToolItem* item_numinstrlabel = manage(new Gtk::ToolItem);
	item_numinstrlabel->add(*manage(new Gtk::Label("Number of instructions : ")));

	Gtk::ToolItem* item_numinstrentry = manage(new Gtk::ToolItem);
	item_numinstrentry->add(m_spinNumInstr);

	Gtk::Toolbar* toolbar = manage(new Gtk::Toolbar);
	toolbar->set_toolbar_style(Gtk::TOOLBAR_ICONS);
	toolbar->set_icon_size(Gtk::ICON_SIZE_SMALL_TOOLBAR);
	toolbar->append(*item_offlabel);
	toolbar->append(*item_offentry);
	toolbar->append(*item_gooff);
	toolbar->append(*tool_separator);
	toolbar->append(*item_numinstrlabel);
	toolbar->append(*item_numinstrentry);

	Gtk::VBox* mainvbox = manage(new Gtk::VBox);
	mainvbox->pack_start(*menubar, false, false);
	mainvbox->pack_start(*toolbar, false, false);
	mainvbox->pack_start(m_treeView, false, false);

	this->add(*mainvbox);
	this->show_all_children();

	m_spinNumInstr.signal_value_changed().connect(sigc::mem_fun(*this,
				&DisassemblerWindow::on_num_instr_changed));
	item_gooff->signal_clicked().connect(sigc::mem_fun(*this,
				&DisassemblerWindow::on_go_to));
	m_entryOffset.signal_activate().connect(sigc::mem_fun(*this,
				&DisassemblerWindow::on_go_to));
}

void DisassemblerWindow::on_num_instr_changed ()
{
	m_treeView.SetNumInstr((uint8_t)m_spinNumInstr.get_value());

	this->resize(this->get_width(), 1);
}

void DisassemblerWindow::on_go_to ()
{
	Glib::ustring text = m_entryOffset.get_text();
	for (Glib::ustring::iterator iter = text.begin(); iter != text.end(); ++iter)
	{
		if (((*iter) < '0' || (*iter) > '9') && ((*iter) < 'A' || (*iter) > 'F')
				&& ((*iter) < 'a' || (*iter) > 'f'))
			return;
	}

	std::istringstream ss(text);
	uint32_t val;

	ss >> std::hex >> val;

	if (!ss.fail())
		m_treeView.GoTo(val);
}

void DisassemblerWindow::on_instruction_set_changed ()
{
	m_treeView.SwitchInstructionSet();
}
