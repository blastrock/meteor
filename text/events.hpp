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

#ifndef __MYM_EVENTS_H__
#define __MYM_EVENTS_H__

#include <ameteor/core.hpp>

#include <SFML/Window.hpp>

class MainText;

namespace mym
{
class Events
{
public:
  Events(sf::Window& window, MainText* m);

  void InitAMeteor(AMeteor::Core& core);

  void CheckEvents();

private:
  sf::Window& m_window;
  MainText* m_maintext;
  AMeteor::Core* m_core;
};
}

#endif
