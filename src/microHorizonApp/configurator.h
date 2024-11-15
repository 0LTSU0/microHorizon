#pragma once

#include "json.hpp"
#include "tracer.h"

enum POSMode
{
	UDP,
	GPS_RECEIVER
};

class MHConfigurator
{
public:
	bool loadConfig();
	std::string getMapPath();
	POSMode getPosMode();

private:
	std::string c_mapPath = "";
	POSMode c_posMode = POSMode::UDP;
};