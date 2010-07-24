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

#include "VramWindow.hpp"
#include <gtkmm/frame.h>
#include <gtkmm/box.h>

#include "ameteor.hpp"

VramWindow::VramWindow () :
	Gtk::Window(),
	m_col16("16"),
	m_col32("32"),
	m_base0("0x06000000"),
	m_base4("0x06004000"),
	m_base8("0x06008000"),
	m_baseC("0x0600C000"),
	m_baseObj("0x06010000")
{
	Gtk::RadioButton::Group grpCol = m_col16.get_group();
	m_col32.set_group(grpCol);
	m_col16.set_active(true);

	Gtk::VBox* boxCol = manage(new Gtk::VBox());
	boxCol->pack_start(m_col16);
	boxCol->pack_start(m_col32);

	Gtk::Frame* frmCol = manage(new Gtk::Frame("Colors"));
	frmCol->add(*boxCol);

	Gtk::RadioButton::Group grpBase = m_base0.get_group();
	m_base4.set_group(grpBase);
	m_base8.set_group(grpBase);
	m_baseC.set_group(grpBase);
	m_baseObj.set_group(grpBase);
	m_base0.set_active(true);

	Gtk::VBox* boxBase = manage(new Gtk::VBox());
	boxBase->pack_start(m_base0);
	boxBase->pack_start(m_base4);
	boxBase->pack_start(m_base8);
	boxBase->pack_start(m_baseC);
	boxBase->pack_start(m_baseObj);

	Gtk::Frame* frmBase = manage(new Gtk::Frame("Char Base"));
	frmBase->add(*boxBase);

	m_palette.set_range(0, 15);
	m_palette.set_value(0);
	m_palette.set_increments(1, 5);

	Gtk::HBox* boxPalette = manage(new Gtk::HBox());
	boxPalette->pack_start(*manage(new Gtk::Label("Palette : ")));
	boxPalette->pack_start(m_palette);

	Gtk::VBox* boxSettings = manage(new Gtk::VBox());
	boxSettings->pack_start(*frmCol);
	boxSettings->pack_start(*frmBase);
	boxSettings->pack_start(*boxPalette);

	m_area.set_size_request(256, 256);

	Gtk::HBox* boxMain = manage(new Gtk::HBox());
	boxMain->pack_start(*boxSettings);
	boxMain->pack_start(m_area);

	this->add(*boxMain);
	this->show_all_children();

	m_col16.signal_toggled().connect(sigc::bind(sigc::mem_fun(*this,
					&VramWindow::on_radio_changed), &m_col16));
	m_col32.signal_toggled().connect(sigc::bind(sigc::mem_fun(*this,
					&VramWindow::on_radio_changed), &m_col32));
	m_base0.signal_toggled().connect(sigc::bind(sigc::mem_fun(*this,
					&VramWindow::on_radio_changed), &m_base0));
	m_base4.signal_toggled().connect(sigc::bind(sigc::mem_fun(*this,
					&VramWindow::on_radio_changed), &m_base4));
	m_base8.signal_toggled().connect(sigc::bind(sigc::mem_fun(*this,
					&VramWindow::on_radio_changed), &m_base8));
	m_baseC.signal_toggled().connect(sigc::bind(sigc::mem_fun(*this,
					&VramWindow::on_radio_changed), &m_baseC));
	m_baseObj.signal_toggled().connect(sigc::bind(sigc::mem_fun(*this,
					&VramWindow::on_radio_changed), &m_baseObj));
	m_palette.signal_value_changed().connect(sigc::mem_fun(*this,
				&VramWindow::on_refresh));
	m_vblank_conn = AMeteor::_lcd.signal_vblank.connect(sigc::mem_fun(*this,
				&VramWindow::on_vblank));
}

VramWindow::~VramWindow ()
{
	m_vblank_conn.disconnect();
}

void VramWindow::on_radio_changed (Gtk::RadioButton* radio)
{
	if (radio->get_active())
		on_refresh();
}

void VramWindow::on_refresh ()
{
	if (!this->is_visible())
		return;

	int8_t palette;
	if (m_col16.get_active())
		palette = (int8_t)m_palette.get_value();
	else
		palette = -1;

	uint32_t base;
	if (m_base0.get_active())
		base = 0x06000000;
	else if (m_base4.get_active())
		base = 0x06004000;
	else if (m_base8.get_active())
		base = 0x06008000;
	else if (m_baseC.get_active())
		base = 0x0600C000;
	else
		base = 0x06010000;

	m_area.Draw(base, palette);
}

void VramWindow::on_vblank()
{
	on_refresh();
}
