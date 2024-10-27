#include "configurator.h"
#include <fstream>

using nlohmann::json;

bool MHConfigurator::loadConfig()
{
	Tracer::log("loadConfig()", traceLevel::DEBUG);
	std::ifstream ifs("C:/Users/lauri/Desktop/microHorizon/config.json"); //TODO: read from runtime dir by default or cmd argument
	json conf = json::parse(ifs);
	if (!conf.contains("mapPath"))
	{
		return false;
	}
	c_mapPath = conf["mapPath"].get<std::string>();
	return true;
}

std::string MHConfigurator::getMapPath()
{
	return c_mapPath;
}