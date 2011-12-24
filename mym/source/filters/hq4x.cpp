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

#include "hq4x.hpp"
#include <algorithm>

namespace mym
{
	namespace Filters
	{
		extern "C"
		{
			unsigned int LUT16to32[65536];
			unsigned int RGBtoYUV[65536];
		}

		//hq4x filter demo program
		//----------------------------------------------------------
		//Copyright (C) 2003 MaxSt ( maxst@hiend3d.com )

		//This program is free software; you can redistribute it and/or
		//modify it under the terms of the GNU Lesser General Public
		//License as published by the Free Software Foundation; either
		//version 2.1 of the License, or (at your option) any later version.
		//
		//This program is distributed in the hope that it will be useful,
		//but WITHOUT ANY WARRANTY; without even the implied warranty of
		//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
		//Lesser General Public License for more details.
		//
		//You should have received a copy of the GNU Lesser General Public
		//License along with this program; if not, write to the Free Software
		//Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA

		int InitLUTs ()
		{
			int i, r, g, b, Y, u, v;

			for (i=0; i<65536; i++)
			{
				r = ((i      ) & 0x1F) << 3;
				g = ((i >>  5) & 0x1F) << 3;
				b = ((i >> 10) & 0x1F) << 3;

				LUT16to32[i] = (r << 16) | (g << 8) | b;

				Y = (r + g + b) >> 2;
				u = 128 + ((r - b) >> 2);
				v = 128 + ((-r + 2*g -b)>>3);
				RGBtoYUV[ i ] =
					(Y<<16) + (u<<8) + v;
			}

			int nMMXsupport = 0;

			asm("mov $1, %%eax\n"
					"cpuid\n"
					"and $0x00800000, %%edx\n"
					"mov %%edx, %0\n"
					:"=r"(nMMXsupport)
					:
					:"rax","rbx","rcx","rdx");

			return nMMXsupport;
		}
	}
}
