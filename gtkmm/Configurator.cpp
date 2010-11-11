#include "Configurator.hpp"
#include <ameteor.hpp>

void Configurator::Load()
{
	m_cfg.LoadFile("meteor.cfg");
}

void Configurator::InitAMeteor()
{
	int tmp;
#define ASSIGN_BUTTON(btn, def) \
	tmp = m_cfg.GetInt("keyboard_" #btn); \
	if (tmp != INT_MIN) \
		AMeteor::_keypad.BindKey(tmp, AMeteor::Keypad::BTN_##btn); \
	else \
		AMeteor::_keypad.BindKey(def, AMeteor::Keypad::BTN_##btn);
	ASSIGN_BUTTON(A     , 119);
	ASSIGN_BUTTON(B     , 120);
	ASSIGN_BUTTON(L     , 113);
	ASSIGN_BUTTON(R     , 115);
	ASSIGN_BUTTON(START , 97);
	ASSIGN_BUTTON(SELECT, 122);
	ASSIGN_BUTTON(LEFT  , 65361);
	ASSIGN_BUTTON(UP    , 65362);
	ASSIGN_BUTTON(RIGHT , 65363);
	ASSIGN_BUTTON(DOWN  , 65364);
#undef ASSIGN_BUTTON

#define ASSIGN_BUTTON(btn) \
	tmp = m_cfg.GetInt("joy_" #btn); \
	if (tmp != INT_MIN) \
		AMeteor::_keypad.BindJoy((tmp >> 8) & 0xFF, tmp & 0xFF, \
				AMeteor::Keypad::BTN_##btn);
	ASSIGN_BUTTON(A     );
	ASSIGN_BUTTON(B     );
	ASSIGN_BUTTON(L     );
	ASSIGN_BUTTON(R     );
	ASSIGN_BUTTON(START );
	ASSIGN_BUTTON(SELECT);
#undef ASSIGN_BUTTON
}
