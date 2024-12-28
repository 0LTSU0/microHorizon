#pragma once

#include "json.hpp"
#include "tracer.h"

using nlohmann::json;

struct cmdArgs
{
	std::string configPath;
};

enum POSMode
{
	UDP,
	GPS_RECEIVER
};

class MHConfigurator
{
public:
	bool loadConfig(std::string&);
	std::string getMapPath();
	POSMode getPosMode();
	int getUDPPort();
	float getLoadRadius();

private:
	std::string c_mapPath = "";
	POSMode c_posMode = POSMode::UDP;
	int c_udpPort = 0;
	float c_loadRadius = 0.5; //radius in degrees (latlon) that is loaded around requested coordinates by OSMProcessor

	bool loadUDPSpecificConfs(json& conf);
	bool loadGPSSpecificConfs(json& conf);
};