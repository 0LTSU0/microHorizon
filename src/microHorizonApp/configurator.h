#pragma once

#include "json.hpp"
#include "tracer.h"

using nlohmann::json;

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
	int getUDPPort();

private:
	std::string c_mapPath = "";
	POSMode c_posMode = POSMode::UDP;
	int c_udpPort = 0;

	bool loadUDPSpecificConfs(json& conf);
	bool loadGPSSpecificConfs(json& conf);
};