#include "Config.hpp"
#include <sstream>
#include <fstream>
#include <limits.h>

void Config::LoadFile(const char* filename)
{
	std::ifstream file(filename);

	char buf[256];
	std::string str, key, val;
	size_t pos;

	while (file.getline(buf, 255, '\n'))
	{
		str = buf;
		if (str.empty() || str[0] == '#')
			continue;
		pos = str.find('=');
		if (pos == str.size())
			continue;
		key = str.substr(0, pos);
		val = str.substr(pos+1);
		m_conf[key] = val;
	}
}

int Config::GetInt(const std::string& key) const
{
	if (!m_conf.count(key))
		return INT_MIN;
	std::istringstream ss;
	ss.str(m_conf.at(key));
	int out;
	ss >> out;
	if (!ss)
		return INT_MIN;
	return out;
}
