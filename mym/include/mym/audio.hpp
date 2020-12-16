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

#ifndef __MYM_AUDIO_HPP__
#define __MYM_AUDIO_HPP__

#include <ao/ao.h>
#include <stdint.h>

namespace mym
{
class Audio
{
public:
  Audio();
  ~Audio();

  void InitAMeteor();
  void Init();
  void Uninit();

  void PlayFrames(const int16_t* data);

  // inline void SetSampleskip (uint16_t skip);

private:
  ao_device* m_device;
};
}

#endif
