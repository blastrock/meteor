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

#include "MainText.hpp"

#include "configurator.hpp"
#include <ameteor/core.hpp>
#include <filesystem>
#include <iostream>
#include <stdexcept>

using namespace AMeteor;

MainText::MainText() : m_events(m_window.GetWindow(), this), m_running(false)
{
  Configurator config;
  config.Load();
  config.InitAMeteor(m_core);

  m_window.InitAMeteor(m_core);
  m_audio.InitAMeteor(m_core);
  m_events.InitAMeteor(m_core);

  m_batteryPath = config.GetBatteryPath();
  m_sstatePath = config.GetSStatePath();
  m_romPath = config.GetRomPath();
}

void MainText::Open(const std::string& path)
{
  std::cout << "Loading " << path << std::endl;

  m_openFile = path;
  {
    std::vector<uint8_t> rom(0x02000000);

    std::ifstream file(path);
    file.read((char*)rom.data(), 0x02000000);
    if (file.bad())
      throw std::runtime_error("Failed to load ROM");

    rom.resize(file.gcount());

    m_core.get<Memory>().LoadRom(rom.data(), rom.size());
  }

  std::string file = (std::filesystem::path(m_batteryPath) /
                      std::filesystem::path(m_openFile).filename())
                         .string();
  if (file[file.size() - 4] == '.')
    file.replace(file.end() - 3, file.end(), "mct");
  else
    file.append(".mct");
  m_core.get<Memory>().SetCartFile(file.c_str());
  if (m_core.get<Memory>().LoadCart() == AMeteor::Memory::CERR_FAIL)
    std::cout << "Failed to load battery" << std::endl;

  std::cout << "Rom loaded" << std::endl;
}

void MainText::OpenBios(const std::string& file)
{
  if (!m_core.get<Memory>().LoadBios(file.c_str()))
    throw std::runtime_error("Failed to load BIOS");
}

void MainText::Close()
{
  m_running = false;

  m_core.Reset(AMeteor::UNIT_ALL ^ AMeteor::UNIT_MEMORY_BIOS);
  m_window.Uninit();
  m_openFile.clear();
}

void MainText::CloseBios()
{
  if (m_running)
    return;

  m_core.Reset(AMeteor::UNIT_MEMORY | AMeteor::UNIT_MEMORY_BIOS);
}

void MainText::Run()
{
  if (m_running)
    return;

  m_window.Init();
  m_audio.Init();

  m_running = true;
  do
  {
    m_core.Run(280000);
  } while (m_running);
}

void MainText::Stop()
{
  m_running = false;
}

void MainText::Reset()
{
  m_core.Reset(
      AMeteor::UNIT_ALL ^
      (AMeteor::UNIT_MEMORY_ROM | AMeteor::UNIT_MEMORY_BIOS));
}

std::string MainText::GetSaveStatePath(uint8_t n)
{
  std::string filename = (std::filesystem::path(m_sstatePath) /
                          std::filesystem::path(m_openFile).filename())
                             .string();
  if (filename[filename.size() - 4] == '.')
    filename.replace(filename.end() - 3, filename.end(), "mst");
  else
    filename.append(".mst");
  filename.insert(filename.end() - 4, 1, n + '0');

  return filename;
}

void MainText::SaveState(uint8_t n)
{
  std::ostringstream ss;

  if (!m_core.SaveState(ss))
  {
    std::cout << "Cannot save state!" << std::endl;
    return;
  }

  std::ofstream file(GetSaveStatePath(n));

  if (!file)
  {
    std::cout << "Cannot save state!" << std::endl;
    return;
  }

  std::string buf = ss.str();
  if (!file.write(buf.c_str(), buf.length()))
  {
    std::cout << "Cannot save state!" << std::endl;
    return;
  }

  file.close();
  if (file.bad())
  {
    std::cout << "Cannot save state!" << std::endl;
    return;
  }
}

void MainText::LoadState(uint8_t n)
{
  std::istringstream ss;
  {
    std::ifstream file(GetSaveStatePath(n));
    if (!file)
    {
      std::cout << "Cannot load state!" << std::endl;
      return;
    }

    // 1Mo
    std::vector<uint8_t> buf(0x100000);
    if (file.read((char*)&buf[0], 0x100000).bad())
    {
      std::cout << "Cannot load state!" << std::endl;
      return;
    }
    int nread = file.gcount();

    file.close();
    if (file.bad())
    {
      std::cout << "Cannot load state!" << std::endl;
      return;
    }

    ss.str(std::string((char*)&buf[0], nread));
  }

  if (!m_core.LoadState(ss))
  {
    std::cout << "Cannot load state!" << std::endl;
    return;
  }
}
