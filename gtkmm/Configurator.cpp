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

#include "Configurator.hpp"
#include "../config.h"
#include <string>
#include <glibmm/miscutils.h>
#include <giomm/file.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

bool Configurator::Load()
{
	std::vector<std::string> path;
	path.push_back(Glib::get_home_dir());
	path.push_back(".meteor");
	path.push_back("meteor.cfg");
	std::string file = Glib::build_filename(path);
	return this->LoadFile(file.c_str());
}

bool Configurator::Save()
{
	std::vector<std::string> path;
	path.push_back(Glib::get_home_dir());
	path.push_back(".meteor");
	path.push_back("meteor.cfg");
	std::string file = Glib::build_filename(path);
	return this->SaveFile(file.c_str());
}

void Configurator::Init()
{
	this->InitAMeteor();

	std::string str;
#define SET_PATH(name, var, def) \
	str = this->GetStr(name); \
	if (str.empty()) \
	{ \
		this->SetStr(name, std::string("~") + def); \
		str = Glib::get_home_dir() + def; \
	} \
	else if (str[0] == '~') \
		str = Glib::get_home_dir() + str.substr(1); \
	m_##var = str; \
	MakePath(m_##var);
	SET_PATH("BatteryPath", batteryPath, "/.meteor/battery");
	SET_PATH("SaveStatePath", sstatePath, "/.meteor/savestate");
	SET_PATH("RomPath", romPath, "");
#undef SET_PATH
}

void Configurator::MakePath(const std::string& path)
{
	std::string::const_iterator begin = path.begin();
	std::string::const_iterator iter = begin;
	struct stat st;
	do
	{
		++iter;
		if (iter == path.end() || *iter == G_DIR_SEPARATOR)
		{
			std::string path(begin, iter);
			if (stat(path.c_str(), &st) == -1)
				if (mkdir(path.c_str(), 0755) == -1)
					return;
		}
	} while (iter != path.end());
}
