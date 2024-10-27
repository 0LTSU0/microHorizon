#pragma once
#include "tracer.h"
#include "configurator.h"

class inputPosition {
public:
	float lat;
	float lon;
};

class OSMProcessor {
public:
	bool init(MHConfigurator&);
	bool matchNewPosition(inputPosition&);
	void loadRoadsAroundLoc(inputPosition& pos);

private:
	std::string mapPath = "";
};