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
#include <filesystem>
#include <string>

namespace
{
std::string _GetHomeDir()
{
  const char* home = getenv("HOME");
  if (!home)
    throw std::runtime_error("can't find HOME environment variable");
  return home;
}

std::string GetHomeDir()
{
  static std::string home = _GetHomeDir();
  return home;
}
}

void Configurator::Load()
{
  std::filesystem::path file =
      std::filesystem::path(_GetHomeDir()) / ".meteor" / "meteor.cfg";
  m_cfg.LoadFile(file.c_str());
}

void Configurator::InitAMeteor(AMeteor::Core& core)
{
  m_cfg.InitAMeteor(core);

  std::string str;
#define SET_PATH(name, var)             \
  str = m_cfg.GetStr(name);             \
  if (!str.empty() && str[0] == '~')    \
    str = GetHomeDir() + str.substr(1); \
  m_##var = str;
  SET_PATH("BatteryPath", batteryPath);
  SET_PATH("SaveStatePath", sstatePath);
  SET_PATH("RomPath", romPath);
#undef SET_PATH
}
