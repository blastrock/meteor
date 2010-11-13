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

#include "MainWindow.hpp"

#include <gtkmm/main.h>
#include <gtkmm/menu.h>
#include <gtkmm/menubar.h>
#include <gtkmm/stock.h>
#include <gtkmm/filechooserdialog.h>
#include <gtkmm/messagedialog.h>

#include "ameteor.hpp"
#include "Configurator.hpp"

#include "debug.hpp"

#include <gdk/gdkx.h>

MainWindow::MainWindow () :
	Gtk::Window(),
	m_renderer(AMeteor::_lcd.GetScreen().GetRenderer()),
	m_mainimage("../../data/meteor.png"),
	m_running(false)
{
	this->set_title("Meteor");
	this->set_resizable(false);

	Gtk::Menu* menu_file = manage(new Gtk::Menu);
	menu_file->items().push_back(Gtk::Menu_Helpers::StockMenuElem(
				Gtk::Stock::OPEN, sigc::mem_fun(*this,
					&MainWindow::on_open)));
	Gtk::Image* img = manage(new Gtk::Image(Gtk::Stock::OPEN,
				Gtk::ICON_SIZE_MENU));
	menu_file->items().push_back(Gtk::Menu_Helpers::ImageMenuElem(
				"Open _BIOS", *img, sigc::mem_fun(*this, &MainWindow::on_open_bios)));
	menu_file->items().push_back(Gtk::Menu_Helpers::StockMenuElem(
				Gtk::Stock::CLOSE, sigc::mem_fun(*this,
					&MainWindow::on_close)));
	img = manage(new Gtk::Image(Gtk::Stock::CLOSE,
				Gtk::ICON_SIZE_MENU));
	menu_file->items().push_back(Gtk::Menu_Helpers::ImageMenuElem(
				"Close B_IOS", *img, sigc::mem_fun(*this,
					&MainWindow::on_close_bios)));
	menu_file->items().push_back(Gtk::Menu_Helpers::SeparatorElem());
	menu_file->items().push_back(Gtk::Menu_Helpers::StockMenuElem(
				Gtk::Stock::QUIT, sigc::mem_fun(*this,
					&MainWindow::on_quit)));

	Gtk::Menu* menu_emulator = manage(new Gtk::Menu);
	img = manage(new Gtk::Image(Gtk::Stock::MEDIA_PLAY,
				Gtk::ICON_SIZE_MENU));
	menu_emulator->items().push_back(Gtk::Menu_Helpers::ImageMenuElem(
				"R_un", Gtk::AccelKey('u', Gdk::CONTROL_MASK), *img,
				sigc::mem_fun(*this, &MainWindow::on_run)));
	img = manage(new Gtk::Image(Gtk::Stock::MEDIA_STOP,
				Gtk::ICON_SIZE_MENU));
	menu_emulator->items().push_back(Gtk::Menu_Helpers::ImageMenuElem(
				"_Stop", Gtk::AccelKey('s', Gdk::CONTROL_MASK), *img,
				sigc::mem_fun(*this, &MainWindow::on_stop)));
	img = manage(new Gtk::Image(Gtk::Stock::MEDIA_REWIND,
				Gtk::ICON_SIZE_MENU));
	menu_emulator->items().push_back(Gtk::Menu_Helpers::ImageMenuElem(
				"_Reset", Gtk::AccelKey('r', Gdk::CONTROL_MASK), *img,
				sigc::mem_fun(*this, &MainWindow::on_reset)));

	Gtk::Menu* menu_sstate = manage(new Gtk::Menu);
	Gtk::Menu* menu_lstate = manage(new Gtk::Menu);
	{
		char tmp[2];
		tmp[0] = '1';
		tmp[1] = '\0';
		for (char c = 0; c < 9; ++c, ++tmp[0])
			menu_sstate->items().push_back(Gtk::Menu_Helpers::MenuElem(
						tmp, Gtk::AccelKey(GDK_F1 + c, Gdk::CONTROL_MASK),
						sigc::bind<uint8_t>(sigc::mem_fun(
								*this, &MainWindow::on_save_state), c)));

		tmp[0] = '1';
		for (char c = 0; c < 9; ++c, ++tmp[0])
			menu_lstate->items().push_back(Gtk::Menu_Helpers::MenuElem(
						tmp, Gtk::AccelKey(GDK_F1 + c, (Gdk::ModifierType)0),
						sigc::bind<uint8_t>(sigc::mem_fun(
								*this, &MainWindow::on_load_state), c)));
	}

	Gtk::Menu* menu_state = manage(new Gtk::Menu);
	img = manage(new Gtk::Image(Gtk::Stock::SAVE,
				Gtk::ICON_SIZE_MENU));
	menu_state->items().push_back(Gtk::Menu_Helpers::ImageMenuElem(
				"Save", *img, *menu_sstate));
	img = manage(new Gtk::Image(Gtk::Stock::OPEN,
				Gtk::ICON_SIZE_MENU));
	menu_state->items().push_back(Gtk::Menu_Helpers::ImageMenuElem(
				"Load", *img, *menu_lstate));

	Gtk::Menu* menu_debug = manage(new Gtk::Menu);
	Gtk::Menu_Helpers::CheckMenuElem disassemblercheck(
				"Disassembler", sigc::mem_fun(*this,
					&MainWindow::on_menu_disassembler_toggle));
	m_refDisassemblerCheck =
		Glib::RefPtr<Gtk::CheckMenuItem>::cast_static(
				disassemblercheck.get_child());
	menu_debug->items().push_back(disassemblercheck);
	Gtk::Menu_Helpers::CheckMenuElem palettecheck(
				"Palette", sigc::mem_fun(*this,
					&MainWindow::on_menu_palette_toggle));
	m_refPaletteCheck =
		Glib::RefPtr<Gtk::CheckMenuItem>::cast_static(
				palettecheck.get_child());
	menu_debug->items().push_back(palettecheck);
	Gtk::Menu_Helpers::CheckMenuElem vramcheck(
				"Vram viewer", sigc::mem_fun(*this,
					&MainWindow::on_menu_vram_toggle));
	m_refVramCheck =
		Glib::RefPtr<Gtk::CheckMenuItem>::cast_static(
				vramcheck.get_child());
	menu_debug->items().push_back(vramcheck);

	Gtk::Menu* menu_help = manage(new Gtk::Menu);
	menu_help->items().push_back(Gtk::Menu_Helpers::StockMenuElem(
				Gtk::Stock::ABOUT, sigc::mem_fun(*this,
					&MainWindow::on_menu_about)));

	Gtk::MenuBar* menubar = manage(new Gtk::MenuBar);
	menubar->items().push_back(Gtk::Menu_Helpers::MenuElem("_File", *menu_file));
	menubar->items().push_back(
			Gtk::Menu_Helpers::MenuElem("_Emulator", *menu_emulator));
	menubar->items().push_back(
			Gtk::Menu_Helpers::MenuElem("_State", *menu_state));
	menubar->items().push_back(
			Gtk::Menu_Helpers::MenuElem("_Debug", *menu_debug));
	menubar->items().push_back(
			Gtk::Menu_Helpers::StockMenuElem(Gtk::Stock::HELP, *menu_help));

	m_viewport.set_double_buffered(false);
	m_viewport.set_size_request(4*240, 4*160);

	m_statusbar.set_has_resize_grip(false);

	m_mainvbox.pack_start(*menubar);
	m_mainvbox.pack_start(m_mainimage);
	m_mainvbox.pack_start(m_viewport);
	m_mainvbox.pack_start(m_statusbar);

	this->add(m_mainvbox);

	menubar->show_all();
	m_mainimage.show_all();
	m_statusbar.show_all();
	m_mainvbox.show();
	this->show();

	m_disassemblerWindow.signal_hide().connect(sigc::mem_fun(*this,
				&MainWindow::on_disassembler_hide));
	m_paletteWindow.signal_hide().connect(sigc::mem_fun(*this,
				&MainWindow::on_palette_hide));
	m_vramWindow.signal_hide().connect(sigc::mem_fun(*this,
				&MainWindow::on_vram_hide));

	Configurator config;
	config.Load();
	config.InitAMeteor(*this);

	//m_refDisassemblerCheck->set_active(true);
	//m_refPaletteCheck->set_active(true);
	m_aboutDialog.set_logo(m_mainimage.get_pixbuf());
}

void MainWindow::on_open ()
{
	Gtk::FileChooserDialog dialog(*this, "Meteor - Choose a ROM");
	dialog.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
	dialog.add_button(Gtk::Stock::OK, Gtk::RESPONSE_OK);
	dialog.set_current_folder(m_romPath);
	if (dialog.run() == Gtk::RESPONSE_OK)
	{
		m_openFile = dialog.get_filename();
		AMeteor::_memory.LoadRom(m_openFile.c_str());

		std::string file = Glib::build_filename(m_batteryPath,
				Glib::path_get_basename(m_openFile));
		if (file[file.size()-4] == '.')
			file.replace(file.end()-3, file.end(), "mct");
		else
			file.append(".mct");
		AMeteor::_memory.SetCartFile(file.c_str());
		AMeteor::_memory.LoadCart();

		m_disassemblerWindow.Reload();
		m_statusbar.push("ROM loaded.");
	}
}

void MainWindow::on_open_bios ()
{
	Gtk::FileChooserDialog dialog(*this, "Meteor - Choose a BIOS");
	dialog.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
	dialog.add_button(Gtk::Stock::OK, Gtk::RESPONSE_OK);
	if (dialog.run() == Gtk::RESPONSE_OK)
	{
		AMeteor::_memory.LoadBios(dialog.get_filename().c_str());

		m_disassemblerWindow.Reload();
		m_statusbar.push("BIOS loaded.");
	}
}

void MainWindow::on_close ()
{
	m_running = false;

	AMeteor::Reset(AMeteor::UNIT_ALL ^ AMeteor::UNIT_MEMORY_BIOS);
	m_renderer.Uninit();
	m_openFile.clear();

	m_viewport.hide();
	m_mainimage.show();
}

void MainWindow::on_close_bios ()
{
	if (m_running)
		return;

	AMeteor::Reset(AMeteor::UNIT_MEMORY | AMeteor::UNIT_MEMORY_BIOS);
}

void MainWindow::on_run ()
{
	if (m_running)
		return;

	// not yet initialized
	if (!m_viewport.is_visible())
	{
		m_mainimage.hide();
		m_viewport.show();

		GtkWidget* widget = GTK_WIDGET(m_viewport.gobj());
		gtk_widget_realize(widget);
		GdkWindow* win = widget->window;
		XFlush(GDK_WINDOW_XDISPLAY(win));
		m_renderer.Init(GDK_WINDOW_XWINDOW(win));
	}

	m_running = true;
	do
	{
		AMeteor::Run(280000);

		while (Gtk::Main::events_pending())
			Gtk::Main::iteration();
	} while (m_running);
}

void MainWindow::on_menu_disassembler_toggle ()
{
	if (m_refDisassemblerCheck->get_active())
		m_disassemblerWindow.show_all();
	else
		m_disassemblerWindow.hide_all();
}

void MainWindow::on_menu_palette_toggle ()
{
	if (m_refPaletteCheck->get_active())
		m_paletteWindow.show_all();
	else
		m_paletteWindow.hide_all();
}

void MainWindow::on_menu_vram_toggle ()
{
	if (m_refVramCheck->get_active())
		m_vramWindow.show_all();
	else
		m_vramWindow.hide_all();
}

void MainWindow::on_disassembler_hide ()
{
	m_refDisassemblerCheck->set_active(false);
}

void MainWindow::on_palette_hide ()
{
	m_refPaletteCheck->set_active(false);
}

void MainWindow::on_vram_hide ()
{
	m_refVramCheck->set_active(false);
}

void MainWindow::on_menu_about ()
{
	m_aboutDialog.run();
}

void MainWindow::on_stop ()
{
	m_running = false;
}

void MainWindow::on_reset ()
{
	AMeteor::Reset(AMeteor::UNIT_ALL ^
			(AMeteor::UNIT_MEMORY_ROM | AMeteor::UNIT_MEMORY_BIOS));
}

void MainWindow::on_quit ()
{
	m_running = false;
	Gtk::Main::quit();
}

void MainWindow::on_save_state (uint8_t n)
{
	std::string file = Glib::build_filename(m_sstatePath,
			Glib::path_get_basename(m_openFile));
	if (file[file.size()-4] == '.')
		file.replace(file.end()-3, file.end(), "mst");
	else
		file.append(".mst");
	file.insert(file.end()-4, 1, n+'0');
	if (!AMeteor::SaveState(file.c_str()))
	{
		Gtk::MessageDialog dlg(*this, "Cannot save state !", false,
				Gtk::MESSAGE_ERROR);
		dlg.run();
	}
	else
		m_statusbar.push("State saved");
}

void MainWindow::on_load_state (uint8_t n)
{
	std::string file = Glib::build_filename(m_sstatePath,
			Glib::path_get_basename(m_openFile));
	if (file[file.size()-4] == '.')
		file.replace(file.end()-3, file.end(), "mst");
	else
		file.append(".mst");
	file.insert(file.end()-4, 1, n+'0');
	if (!AMeteor::LoadState(file.c_str()))
	{
		Gtk::MessageDialog dlg(*this, "Cannot load state !", false,
				Gtk::MESSAGE_ERROR);
		dlg.run();
	}
	else
		m_statusbar.push("State loaded");
}

bool MainWindow::on_delete_event(GdkEventAny* event __attribute((unused)))
{
	on_quit();
	return true;
}

bool MainWindow::on_key_press_event(GdkEventKey* key)
{
	AMeteor::_keypad.KeyPressed(key->keyval);
	return Gtk::Window::on_key_press_event(key);
}

bool MainWindow::on_key_release_event(GdkEventKey* key)
{
	AMeteor::_keypad.KeyReleased(key->keyval);
	return Gtk::Window::on_key_release_event(key);
}
