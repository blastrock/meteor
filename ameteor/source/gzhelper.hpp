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

#ifndef __GZ_HELPER_H__
#define __GZ_HELPER_H__

namespace AMeteor
{
	class GzHelper
	{
		public :
			GzHelper (gzFile file) :
				m_file(file)
			{
			}

			~GzHelper ()
			{
				if (m_file)
					gzclose(m_file);
			}

			operator bool()
			{
				return m_file;
			}

			operator gzFile()
			{
				return m_file;
			}

		private :
			GzHelper (const GzHelper& helper __attribute__((unused)))
			{
			}

			gzFile m_file;
	};
}

#endif
