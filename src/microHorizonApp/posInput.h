#pragma once

#include <string>

class inputPosition {
public:
	float lat;
	float lon;
	float heading = 0.0;
	std::stringstream getInputPosString();
};