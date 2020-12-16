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

#include "mym/config.hpp"
#include <ameteor.hpp>
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

void Config::InitAMeteor()
{
  AMeteor::_keypad.ResetBindings();

  int tmp;
#define ASSIGN_BUTTON(btn, def)                                \
  tmp = GetInt("keyboard_" #btn);                              \
  if (tmp != INT_MIN)                                          \
    AMeteor::_keypad.BindKey(tmp, AMeteor::Keypad::BTN_##btn); \
  else                                                         \
  {                                                            \
    SetInt("keyboard_" #btn, def);                             \
    AMeteor::_keypad.BindKey(def, AMeteor::Keypad::BTN_##btn); \
  }
  ASSIGN_BUTTON(A, 0x77);
  ASSIGN_BUTTON(B, 0x78);
  ASSIGN_BUTTON(L, 0x71);
  ASSIGN_BUTTON(R, 0x73);
  ASSIGN_BUTTON(START, 0x61);
  ASSIGN_BUTTON(SELECT, 0x7A);
  ASSIGN_BUTTON(LEFT, 0xFF51);
  ASSIGN_BUTTON(UP, 0xFF52);
  ASSIGN_BUTTON(RIGHT, 0xFF53);
  ASSIGN_BUTTON(DOWN, 0xFF54);
#undef ASSIGN_BUTTON

#define ASSIGN_BUTTON(btn)    \
  tmp = GetInt("joy_" #btn);  \
  if (tmp != INT_MIN)         \
    AMeteor::_keypad.BindJoy( \
        (tmp >> 16) & 0xFFFF, tmp & 0xFFFF, AMeteor::Keypad::BTN_##btn);
  ASSIGN_BUTTON(A);
  ASSIGN_BUTTON(B);
  ASSIGN_BUTTON(L);
  ASSIGN_BUTTON(R);
  ASSIGN_BUTTON(START);
  ASSIGN_BUTTON(SELECT);
  ASSIGN_BUTTON(LEFT);
  ASSIGN_BUTTON(UP);
  ASSIGN_BUTTON(RIGHT);
  ASSIGN_BUTTON(DOWN);
#undef ASSIGN_BUTTON

#define ASSIGN_BUTTON(btn)       \
  tmp = GetInt("joyaxis_" #btn); \
  if (tmp != INT_MIN)            \
    AMeteor::_keypad.BindAxis(   \
        (tmp >> 16) & 0xFFFF, tmp & 0xFFFF, AMeteor::Keypad::BTN_##btn);
  ASSIGN_BUTTON(A);
  ASSIGN_BUTTON(B);
  ASSIGN_BUTTON(L);
  ASSIGN_BUTTON(R);
  ASSIGN_BUTTON(START);
  ASSIGN_BUTTON(SELECT);
  ASSIGN_BUTTON(LEFT);
  ASSIGN_BUTTON(UP);
  ASSIGN_BUTTON(RIGHT);
  ASSIGN_BUTTON(DOWN);
#undef ASSIGN_BUTTON
}
}
