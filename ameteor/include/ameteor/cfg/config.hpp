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

#ifndef __CONFIG_HPP__
#define __CONFIG_HPP__

#include <map>
#include <string>

namespace AMeteor
{
	namespace Cfg
	{
		class Config
		{
			public:
				void Clear()
				{
					m_conf.clear();
				}

				bool LoadFile(const char* file);
				bool SaveFile(const char* file) const;

				const std::string& GetStr(const std::string& key) const
				{
					if (m_conf.count(key))
						return m_conf.at(key);
					else
						return _empty_string_;
				}
				void SetStr(const std::string& key, const std::string& val)
				{
					m_conf[key] = val;
				}

				int GetInt(const std::string& key) const;
				void SetInt(const std::string& key, int val);

				void InitAMeteor();

			private:
				typedef std::map<std::string, std::string> Config_t;

				static const std::string _empty_string_;
				Config_t m_conf;
		};
	}
}

#endif
