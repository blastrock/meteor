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

#ifndef __CONFIGURATOR_HPP__
#define __CONFIGURATOR_HPP__

#include <mym/config.hpp>

class Configurator
{
public:
  void Load();

  void InitAMeteor(AMeteor::Core& core);

  const std::string& GetBatteryPath() const
  {
    return m_batteryPath;
  }
  const std::string& GetSStatePath() const
  {
    return m_sstatePath;
  }
  const std::string& GetRomPath() const
  {
    return m_romPath;
  }

private:
  mym::Config m_cfg;

  std::string m_batteryPath;
  std::string m_sstatePath;
  std::string m_romPath;
};

#endif
