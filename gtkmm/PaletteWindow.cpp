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

#include "PaletteWindow.hpp"
#include "globals.hpp"

#include "ameteor.hpp"

#include <gtkmm/box.h>
#include <gtkmm/frame.h>

PaletteWindow::PaletteWindow () :
	Gtk::Window()
{
	this->set_title("Meteor - Palette");
	this->set_resizable(false);

	m_bgPalette.set_size_request(161, 161);
	m_objPalette.set_size_request(161, 161);

	Gtk::Frame* bgframe = manage(new Gtk::Frame("BG palette"));
	bgframe->add(m_bgPalette);
	Gtk::Frame* objframe = manage(new Gtk::Frame("OBJ palette"));
	objframe->add(m_objPalette);

	Gtk::HBox* mainhbox = manage(new Gtk::HBox);
	mainhbox->pack_start(*bgframe);
	mainhbox->pack_start(*objframe);

	this->add(*mainhbox);

	m_bgPalette.signal_expose_event().connect(sigc::mem_fun(*this,
				&PaletteWindow::on_bg_expose));
	m_objPalette.signal_expose_event().connect(sigc::mem_fun(*this,
				&PaletteWindow::on_obj_expose));
	m_vblank_conn = AMeteor::_lcd.signal_vblank.connect(sigc::mem_fun(*this,
				&PaletteWindow::on_vblank));
}

PaletteWindow::~PaletteWindow ()
{
	m_vblank_conn.disconnect();
}

void PaletteWindow::DrawGrid (Glib::RefPtr<Gdk::Window> area,
		Glib::RefPtr<Gdk::GC>& gc)
{
	gc->set_rgb_fg_color(Gdk::Color("black"));

	area->draw_rectangle(gc, true, 0, 0, 161, 161);

	gc->set_rgb_fg_color(Gdk::Color("white"));
	for (uint8_t c = 0; c <= 16; ++c)
	{
		area->draw_line(gc, c*10, 0, c*10, 161);
		area->draw_line(gc, 0, c*10, 161, c*10);
	}
}

bool PaletteWindow::on_bg_expose (GdkEventExpose* event __attribute((unused)))
{
	if (!m_bgGc)
		m_bgGc = Gdk::GC::create(m_bgPalette.get_window());

	DrawGrid(m_bgPalette.get_window(), m_bgGc);

	return true;
}

bool PaletteWindow::on_obj_expose (GdkEventExpose* event __attribute((unused)))
{
	if (!m_objGc)
		m_objGc = Gdk::GC::create(m_objPalette.get_window());

	DrawGrid(m_objPalette.get_window(), m_objGc);

	return true;
}

void PaletteWindow::on_vblank ()
{
	if (!this->is_visible())
		return;

	if (!m_bgGc)
		m_bgGc = Gdk::GC::create(m_bgPalette.get_window());
	if (!m_objGc)
		m_objGc = Gdk::GC::create(m_objPalette.get_window());

	uint16_t* palette = (uint16_t*)AMeteor::_memory.GetRealAddress(0x05000000);
	uint32_t icolor;
	Gdk::Color color;

	for (uint16_t i = 0; i < 256; ++i, ++palette)
	{
		icolor = COLORS_16_32(*palette);
		color.set_rgb(
				INT_8_16((icolor >> 16)       ),
				INT_8_16((icolor >>  8) & 0xFF),
				INT_8_16((icolor      ) & 0xFF));
		m_bgGc->set_rgb_fg_color(color);
		m_bgPalette.get_window()->draw_rectangle(m_bgGc, true,
				(i%16)*10+1, (i/16)*10+1, 9, 9);
	}

	for (uint16_t i = 0; i < 256; ++i, ++palette)
	{
		icolor = COLORS_16_32(*palette);
		color.set_rgb(
				INT_8_16((icolor >> 16)       ),
				INT_8_16((icolor >>  8) & 0xFF),
				INT_8_16((icolor      ) & 0xFF));
		m_objGc->set_rgb_fg_color(color);
		m_objPalette.get_window()->draw_rectangle(m_objGc, true,
				(i%16)*10+1, (i/16)*10+1, 9, 9);
	}
}
