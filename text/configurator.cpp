#include "configurator.hpp"
#include <string>
#include <glibmm/miscutils.h>

void Configurator::Load()
{
	std::vector<std::string> path;
	path.push_back(Glib::get_home_dir());
	path.push_back(".meteor");
	path.push_back("meteor.cfg");
	std::string file = Glib::build_filename(path);
	m_cfg.LoadFile(file.c_str());
}

void Configurator::InitAMeteor()
{
	m_cfg.InitAMeteor();

	std::string str;
#define SET_PATH(name, var) \
	str = m_cfg.GetStr(name); \
	if (!str.empty() && str[0] == '~') \
		str = Glib::get_home_dir() + str.substr(1); \
	m_##var = str;
	SET_PATH("BatteryPath", batteryPath);
	SET_PATH("SaveStatePath", sstatePath);
	SET_PATH("RomPath", romPath);
#undef SET_PATH
}
