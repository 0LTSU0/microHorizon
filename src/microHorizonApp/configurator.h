#pragma once

#include "json.hpp"
#include "tracer.h"

class MHConfigurator
{
public:
	bool loadConfig();
	std::string getMapPath();

private:
	std::string c_mapPath = "";
};