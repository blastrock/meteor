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

#include "config.hpp"
#include <ameteor/core.hpp>
#include <fstream>
#include <limits.h>
#include <sstream>

namespace mym
{
const std::string Config::_empty_string_;

bool Config::LoadFile(const char* filename)
{
  std::ifstream file(filename);
  if (!file)
    return false;

  char buf[256];
  std::string str;
  size_t pos;

  while (file.getline(buf, 255, '\n'))
  {
    str = buf;
    if (str.empty() || str[0] == '#' || str[0] == ';')
      continue;
    pos = str.find('=');
    if (pos == std::string::npos)
      continue;
    m_conf[str.substr(0, pos)] = str.substr(pos + 1);
  }

  return true;
}

bool Config::SaveFile(const char* filename) const
{
  std::ofstream file(filename);
  if (!file)
    return false;

  for (Config_t::const_iterator iter = m_conf.begin(); iter != m_conf.end();
       ++iter)
  {
    file << iter->first << '=' << iter->second << '\n';
    if (!file)
      return false;
  }

  return true;
}

int Config::GetInt(const std::string& key) const
{
  if (!m_conf.count(key))
    return INT_MIN;
  std::istringstream ss;
  ss.str(m_conf.at(key));
  int out;
  ss >> std::hex >> out;
  if (!ss)
    return INT_MIN;
  return out;
}

void Config::SetInt(const std::string& key, int val)
{
  std::ostringstream ss;
  ss << std::hex << val;
  m_conf[key] = ss.str();
}
}
