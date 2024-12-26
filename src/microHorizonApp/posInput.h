#pragma once

#include <string>

class inputPosition {
public:
	float lat = 0.0;
	float lon = 0.0;
	float heading = 0.0;
	float speed = 0.0;
	std::stringstream getInputPosString();

	bool operator==(const inputPosition& otherInst) const {
		return lat == otherInst.lat &&
			lon == otherInst.lon &&
			heading == otherInst.heading &&
			speed == otherInst.speed;
	}

	bool operator!=(const inputPosition& other) const {
		return !(*this == other);
	}
};