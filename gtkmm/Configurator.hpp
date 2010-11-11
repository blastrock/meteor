#ifndef __CONFIGURATOR_HPP__
#define __CONFIGURATOR_HPP__

#include "Config.hpp"

class Configurator
{
	public:
		void Load();

		void InitAMeteor(class MainWindow& window);

	private:
		Config m_cfg;
};

#endif
