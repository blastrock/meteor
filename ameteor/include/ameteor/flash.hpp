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

#ifndef __FLASH_H__
#define __FLASH_H__

#include "cartmem.hpp"
#include <stdint.h>
#include <zlib.h>

namespace AMeteor
{
	class Flash : public CartMem
	{
		public :
			Flash (bool big);
			~Flash ();

			void Reset ();

			bool Load (std::istream& f);
			bool Save (std::ostream& f);

			uint8_t Read (uint16_t add);
			bool Write (uint16_t add, uint8_t val);

			bool SaveState (gzFile file);
			bool LoadState (gzFile file);

		private :
			uint8_t m_device_id;
			uint8_t m_manufacturer_id;
			uint32_t m_size;

			enum State
			{
				NORMAL,
				CMD1,
				CMD2,
				ID,
				ERASE1,
				ERASE2,
				ERASE3,
				WRITE
			};

			uint8_t* m_data;
			State m_state;
	};
}

#endif
