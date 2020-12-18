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

#ifndef __MAIN_TEXT_H__
#define __MAIN_TEXT_H__

#include <ameteor/core.hpp>
#include <mym/audio.hpp>
#include <mym/events.hpp>
#include <mym/window.hpp>
#include <string>

class MainText
{
public:
  MainText();

  void Open(const std::string& file);
  void OpenBios(const std::string& file);
  void Run();
  void Stop();
  void Reset();
  void Close();
  void CloseBios();

  void SaveState(uint8_t n);
  void LoadState(uint8_t n);

private:
  AMeteor::Core m_core;

  mym::Window m_window;
  mym::Audio m_audio;
  mym::Events m_events;

  std::string m_sstatePath, m_batteryPath, m_romPath;
  std::string m_openFile;

  bool m_running;
};

#endif
