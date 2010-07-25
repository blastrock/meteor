#ifndef __CONFIG_HPP__
#define __CONFIG_HPP__

#include <map>
#include <string>

class Config
{
	public:
		void Clear()
		{
			m_conf.clear();
		}

		void LoadFile(const char* file);

		const std::string& GetStr(const std::string& key) const
		{
			if (m_conf.count(key))
				return m_conf.at(key);
			else
				return _empty_string_;
		}

		int GetInt(const std::string& key) const;

	private:
		static const std::string _empty_string_;
		std::map<std::string, std::string> m_conf;
};

#endif
