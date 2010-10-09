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

#include "debug.hpp"

#include <gdk/gdkx.h>

MainWindow::MainWindow () :
	Gtk::Window(),
	m_renderer(AMeteor::_lcd.GetScreen().GetRenderer()),
	m_action(ACT_NOTHING),
	m_sstate(0),
	m_mainimage("../../data/meteor.png")
{
	this->set_title("Meteor");
	this->set_resizable(false);

	Gtk::Menu* menu_file = manage(new Gtk::Menu);
	menu_file->items().push_back(Gtk::Menu_Helpers::StockMenuElem(
				Gtk::Stock::OPEN, sigc::mem_fun(*this,
					&MainWindow::on_open)));
	menu_file->items().push_back(Gtk::Menu_Helpers::StockMenuElem(
				Gtk::Stock::CLOSE, sigc::mem_fun(*this,
					&MainWindow::on_close)));
	menu_file->items().push_back(Gtk::Menu_Helpers::SeparatorElem());
	menu_file->items().push_back(Gtk::Menu_Helpers::StockMenuElem(
				Gtk::Stock::QUIT, sigc::mem_fun(*this,
					&MainWindow::on_quit)));

	Gtk::Menu* menu_emulator = manage(new Gtk::Menu);
	Gtk::Image* img = manage(new Gtk::Image(Gtk::Stock::MEDIA_PLAY,
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

	AMeteor::_lcd.signal_vblank.connect(sigc::mem_fun(*this,
				&MainWindow::on_vblank));

	m_config.LoadFile("meteor.cfg");

	int tmp;
#define ASSIGN_BUTTON(btn) \
	tmp = m_config.GetInt("keyboard_" #btn); \
	if (tmp != INT_MIN) \
		AMeteor::_keypad.BindKey(tmp, AMeteor::Keypad::BTN_##btn);
	ASSIGN_BUTTON(A);
	ASSIGN_BUTTON(B);
	ASSIGN_BUTTON(L);
	ASSIGN_BUTTON(R);
	ASSIGN_BUTTON(START);
	ASSIGN_BUTTON(SELECT);
	ASSIGN_BUTTON(LEFT);
	ASSIGN_BUTTON(RIGHT);
	ASSIGN_BUTTON(UP);
	ASSIGN_BUTTON(DOWN);
#undef ASSIGN_BUTTON

	//m_refDisassemblerCheck->set_active(true);
	//m_refPaletteCheck->set_active(true);
}

void MainWindow::on_open ()
{
	/*Gtk::FileChooserDialog dialog(*this, "Meteor - Chose a ROM");
	dialog.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
	dialog.add_button(Gtk::Stock::OK, Gtk::RESPONSE_OK);
	if (dialog.run() == Gtk::RESPONSE_OK)
	{
		m_ameteor.GetMemory()->LoadRom(dialog.get_filename().c_str());
		m_disassemblerWindow.Reload();
		m_statusbar.push("ROM loaded.");
	}*/
		//AMeteor::_memory.LoadBios("/home/blastrock/GBA.BIOS");
		AMeteor::_memory.LoadRom("/home/blastrock/gba/g3.gba");
		AMeteor::_memory.LoadCart("/home/blastrock/gba/g3.sav");
		//AMeteor::_memory.LoadRom("/home/blastrock/GBA.BIOS");
		//AMeteor::_memory.LoadRom("/home/blastrock/vba_bios");
		m_disassemblerWindow.Reload();
		m_statusbar.push("ROM loaded.");
}

void MainWindow::on_close ()
{
	if (AMeteor::_cpu.IsRunning())
	{
		m_action = ACT_CLOSE;
		AMeteor::_cpu.Stop();
	}
	else
	{
		CloseRom();
	}
}

void MainWindow::CloseRom()
{
	AMeteor::Reset();
	m_renderer.Uninit();

	m_viewport.hide();
	m_mainimage.show();
}

void MainWindow::on_run ()
{
	if (AMeteor::_cpu.IsRunning())
		return;

	m_mainimage.hide();
	m_viewport.show();

	GtkWidget* widget = GTK_WIDGET(m_viewport.gobj());
	gtk_widget_realize(widget);
	GdkWindow* win = widget->window;
	XFlush(GDK_WINDOW_XDISPLAY(win));
	m_renderer.Init(GDK_WINDOW_XWINDOW(win));

	while (true)
	{
		AMeteor::Run();

		// The emulator has been stopped
		switch (m_action)
		{
			case ACT_SAVE:
				SaveState(m_sstate);
				break;
			case ACT_LOAD:
				LoadState(m_sstate);
				break;
			case ACT_CLOSE:
				m_action = ACT_NOTHING;
				CloseRom();
				return;
			default:
				return; // nothing to do
		}
		m_action = ACT_NOTHING;
		m_sstate = 0;
	}
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
}

void MainWindow::on_vblank ()
{
	while (Gtk::Main::events_pending())
		Gtk::Main::iteration();
}

void MainWindow::on_stop ()
{
	AMeteor::Stop();
}

void MainWindow::on_reset ()
{
	AMeteor::Reset();
}

void MainWindow::on_quit ()
{
	AMeteor::Stop();
	Gtk::Main::quit();
}

void MainWindow::on_save_state (uint8_t n)
{
	if (AMeteor::_cpu.IsRunning())
	{
		m_action = ACT_SAVE;
		m_sstate = n;
		AMeteor::_cpu.Stop();
	}
	else
		SaveState(n);
}

void MainWindow::SaveState(uint8_t n)
{
	char file[] = "ssX.mst";
	file[2] = n+'1';
	if (!AMeteor::SaveState(file))
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
	if (AMeteor::_cpu.IsRunning())
	{
		m_action = ACT_LOAD;
		m_sstate = n;
		AMeteor::_cpu.Stop();
	}
	else
		LoadState(n);
}

void MainWindow::LoadState(uint8_t n)
{
	char file[] = "ssX.mst";
	file[2] = n+'1';
	if (!AMeteor::LoadState(file))
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
