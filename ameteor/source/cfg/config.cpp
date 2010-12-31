#include "ameteor/cfg/config.hpp"
#include "ameteor.hpp"
#include <sstream>
#include <fstream>
#include <limits.h>

namespace AMeteor
{
	namespace Cfg
	{
		const std::string Config::_empty_string_;

		bool Config::LoadFile(const char* filename)
		{
			std::ifstream file(filename);
			if (!file)
				return false;

			char buf[256];
			std::string str, key, val;
			size_t pos;

			while (file.getline(buf, 255, '\n'))
			{
				str = buf;
				if (str.empty() || str[0] == '#' || str[0] == ';')
					continue;
				pos = str.find('=');
				if (pos == std::string::npos)
					continue;
				key = str.substr(0, pos);
				val = str.substr(pos+1);
				m_conf[key] = val;
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

		void Config::InitAMeteor()
		{
			int tmp;
#define ASSIGN_BUTTON(btn, def) \
	tmp = GetInt("keyboard_" #btn); \
	if (tmp != INT_MIN) \
		AMeteor::_keypad.BindKey(tmp, AMeteor::Keypad::BTN_##btn); \
	else \
		AMeteor::_keypad.BindKey(def, AMeteor::Keypad::BTN_##btn);
			ASSIGN_BUTTON(A     , 0x77);
			ASSIGN_BUTTON(B     , 0x78);
			ASSIGN_BUTTON(L     , 0x71);
			ASSIGN_BUTTON(R     , 0x73);
			ASSIGN_BUTTON(START , 0x61);
			ASSIGN_BUTTON(SELECT, 0x7A);
			ASSIGN_BUTTON(LEFT  , 0xFF51);
			ASSIGN_BUTTON(UP    , 0xFF52);
			ASSIGN_BUTTON(RIGHT , 0xFF53);
			ASSIGN_BUTTON(DOWN  , 0xFF54);
#undef ASSIGN_BUTTON

#define ASSIGN_BUTTON(btn) \
	tmp = GetInt("joy_" #btn); \
	if (tmp != INT_MIN) \
		AMeteor::_keypad.BindJoy((tmp >> 16) & 0xFFFF, tmp & 0xFFFF, \
				AMeteor::Keypad::BTN_##btn);
			ASSIGN_BUTTON(A     );
			ASSIGN_BUTTON(B     );
			ASSIGN_BUTTON(L     );
			ASSIGN_BUTTON(R     );
			ASSIGN_BUTTON(START );
			ASSIGN_BUTTON(SELECT);
			ASSIGN_BUTTON(LEFT  );
			ASSIGN_BUTTON(UP    );
			ASSIGN_BUTTON(RIGHT );
			ASSIGN_BUTTON(DOWN  );
#undef ASSIGN_BUTTON

#define ASSIGN_BUTTON(btn) \
	tmp = GetInt("joyaxis_" #btn); \
	if (tmp != INT_MIN) \
		AMeteor::_keypad.BindAxis((tmp >> 16) & 0xFFFF, tmp & 0xFFFF, \
				AMeteor::Keypad::BTN_##btn);
			ASSIGN_BUTTON(A     );
			ASSIGN_BUTTON(B     );
			ASSIGN_BUTTON(L     );
			ASSIGN_BUTTON(R     );
			ASSIGN_BUTTON(START );
			ASSIGN_BUTTON(SELECT);
			ASSIGN_BUTTON(LEFT  );
			ASSIGN_BUTTON(UP    );
			ASSIGN_BUTTON(RIGHT );
			ASSIGN_BUTTON(DOWN  );
#undef ASSIGN_BUTTON
		}
	}
}
