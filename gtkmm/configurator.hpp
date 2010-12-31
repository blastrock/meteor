#ifndef __CONFIGURATOR_HPP__
#define __CONFIGURATOR_HPP__

#include <ameteor/cfg/config.hpp>

class Configurator
{
	public:
		void Load();

		void InitAMeteor();

		const std::string& GetBatteryPath() const
		{
			return m_batteryPath;
		}
		const std::string& GetSStatePath() const
		{
			return m_sstatePath;
		}
		const std::string& GetRomPath() const
		{
			return m_romPath;
		}

	private:
		AMeteor::Cfg::Config m_cfg;

		std::string m_batteryPath;
		std::string m_sstatePath;
		std::string m_romPath;
};

#endif
