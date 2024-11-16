#include "configurator.h"
#include <fstream>

bool MHConfigurator::loadConfig()
{
	Tracer::log("loadConfig()", traceLevel::DEBUG);
	std::ifstream ifs("C:/Users/lauri/Desktop/microHorizon/config.json"); //TODO: read from runtime dir by default or cmd argument
	json conf = json::parse(ifs);
	if (!conf.contains("mapPath") || !conf.contains("posMode")) //TODO: use json schema
	{
		return false;
	}

	// map path
	c_mapPath = conf["mapPath"].get<std::string>();

	// positioning mode
	if (conf["posMode"].get<std::string>() == "UDP")
	{
		c_posMode = POSMode::UDP;
		if (!loadUDPSpecificConfs(conf))
		{
			return false;
		}
	}
	else if (conf["posMode"].get<std::string>() == "GPS")
	{
		c_posMode = POSMode::GPS_RECEIVER;
		if (!loadGPSSpecificConfs(conf))
		{
			return false;
		}
	}
	else
	{
		return false;
	}

	return true;
}

std::string MHConfigurator::getMapPath()
{
	return c_mapPath;
}

POSMode MHConfigurator::getPosMode()
{
	return c_posMode;
}

int MHConfigurator::getUDPPort()
{
	return c_udpPort;
}

bool MHConfigurator::loadUDPSpecificConfs(json& conf)
{
	if (!conf.contains("UDPPosPort"))
	{
		return false;
	}
	try {
		c_udpPort = conf["UDPPosPort"].get<int>();
	}
	catch (const json::type_error& e)
	{
		return false;
	}
	
	return true;
}

bool MHConfigurator::loadGPSSpecificConfs(json& conf)
{
	return false; //todo when implementing gps receiver support
}
