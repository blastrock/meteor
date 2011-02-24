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

#include "configurator.hpp"
#include <string>
#include <glibmm/miscutils.h>

void Configurator::Load()
{
	std::vector<std::string> path;
	path.push_back(Glib::get_home_dir());
	path.push_back(".meteor");
	path.push_back("meteor.cfg");
	std::string file = Glib::build_filename(path);
	m_cfg.LoadFile(file.c_str());
}

void Configurator::InitAMeteor()
{
	m_cfg.InitAMeteor();

	std::string str;
#define SET_PATH(name, var, def) \
	str = m_cfg.GetStr(name); \
	if (str.empty()) \
		str = Glib::get_home_dir() + def; \
	else if (str[0] == '~') \
		str = Glib::get_home_dir() + str.substr(1); \
	m_##var = str;
	SET_PATH("BatteryPath", batteryPath, "/.meteor/battery");
	SET_PATH("SaveStatePath", sstatePath, "/.meteor/savestate");
	SET_PATH("RomPath", romPath, "");
#undef SET_PATH
}
