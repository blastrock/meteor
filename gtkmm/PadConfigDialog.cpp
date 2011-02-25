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

#include "PadConfigDialog.hpp"

#include <gtkmm/table.h>
#include <gtkmm/label.h>
#include <gtkmm/stock.h>

static char const * const Labels[] = {
	"A",
	"B",
	"L",
	"R",
	"Start",
	"Select",
	"Up",
	"Down",
	"Left",
	"Right",
	"Save state path",
	"Battery path",
	"Rom path"
};

PadConfigDialog::PadConfigDialog(Gtk::Window& parent, Configurator& conf) :
	Gtk::Dialog("Pad configuration", parent, true, true),
	m_conf(conf),
	m_binding(-1)
{
	Gtk::Table* mainTable = manage(new Gtk::Table(13, 2));

	for (unsigned char c = 0; c < NB_BTN; ++c)
	{
		mainTable->attach(*manage(new Gtk::Label(Labels[c])),
				0, 1, c, c+1, Gtk::FILL | Gtk::EXPAND, (Gtk::AttachOptions)0);
		mainTable->attach(m_buttons[c],
				1, 2, c, c+1, Gtk::FILL | Gtk::EXPAND, (Gtk::AttachOptions)0);
		m_buttons[c].signal_clicked().connect(sigc::bind<unsigned char>(
					sigc::mem_fun(*this, &PadConfigDialog::on_bind), c));

	}

	for (unsigned char c = 0; c < NB_ETR; ++c)
	{
		mainTable->attach(*manage(new Gtk::Label(Labels[c+NB_BTN])),
				0, 1, c+NB_BTN, c+NB_BTN+1,
				Gtk::FILL | Gtk::EXPAND, (Gtk::AttachOptions)0);
		mainTable->attach(m_entries[c],
				1, 2, c+NB_BTN, c+NB_BTN+1,
				Gtk::FILL | Gtk::EXPAND, (Gtk::AttachOptions)0);
	}

	Gtk::VBox* vbox = this->get_vbox();
	vbox->pack_start(*mainTable);

	this->add_button(Gtk::Stock::OK, Gtk::RESPONSE_OK);
	this->add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);

	this->show_all_children();
}

void PadConfigDialog::on_show()
{
	m_newConf = m_conf;

	for (unsigned char c = 0; c < NB_BTN; ++c)
		m_buttons[c].set_label(gdk_keyval_name(m_conf.GetInt(
						"keyboard_" + Glib::ustring(Labels[c]).uppercase())));

	m_entries[ETR_BATTERY].set_text(m_conf.GetStr("BatteryPath"));
	m_entries[ETR_SSTATE].set_text(m_conf.GetStr("SaveStatePath"));
	m_entries[ETR_ROMS].set_text(m_conf.GetStr("RomPath"));

	Gtk::Dialog::on_show();
}

void PadConfigDialog::on_response(int resp)
{
	if (resp == Gtk::RESPONSE_OK)
	{
		m_conf.SetStr("BatteryPath", m_entries[ETR_BATTERY].get_text());
		m_conf.SetStr("SaveStatePath", m_entries[ETR_SSTATE].get_text());
		m_conf.SetStr("RomPath", m_entries[ETR_ROMS].get_text());
		m_conf = m_newConf;
	}

	Gtk::Dialog::on_response(resp);

	this->hide();
}

void PadConfigDialog::on_bind(unsigned char btn)
{
	m_binding = btn;
}

bool PadConfigDialog::on_key_press_event(GdkEventKey* ev)
{
	if (m_binding == -1)
		return Gtk::Dialog::on_key_press_event(ev);

	if (ev->keyval != GDK_Escape)
	{
		m_newConf.SetInt("keyboard_" +
				Glib::ustring(Labels[(unsigned char)m_binding]).uppercase(),
				ev->keyval);
		m_buttons[(unsigned char)m_binding].set_label(gdk_keyval_name(ev->keyval));
	}

	m_buttons[(unsigned char)m_binding].set_active(false);
	m_binding = -1;

	return true;
}
