#include "Configurator.hpp"
#include "MainWindow.hpp"
#include <ameteor.hpp>
#include <string>

void Configurator::Load()
{
	m_cfg.LoadFile("meteor.cfg");
}

void Configurator::InitAMeteor(MainWindow& window)
{
	int tmp;
#define ASSIGN_BUTTON(btn, def) \
	tmp = m_cfg.GetInt("keyboard_" #btn); \
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
	tmp = m_cfg.GetInt("joy_" #btn); \
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
	tmp = m_cfg.GetInt("joyaxis_" #btn); \
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

	std::string str;
#define SET_PATH(name, var) \
	str = m_cfg.GetStr(name); \
	if (!str.empty() && str[0] == '~') \
		str = Glib::get_home_dir() + str.substr(1); \
	window.m_##var = str;
	SET_PATH("BatteryPath", batteryPath);
	SET_PATH("SaveStatePath", sstatePath);
	SET_PATH("RomPath", romPath);
#undef SET_PATH
}
