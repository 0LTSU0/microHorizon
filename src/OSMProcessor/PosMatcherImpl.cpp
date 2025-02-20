//#include "OSMProcessor.h"
#include "PosMatcherImpl.h"
#include <cmath>
#include <numbers>
#include <set>
#include <fstream>
#include <limits>
#include "tracer.h"
#include "json.hpp"

using nlohmann::json;

struct Point {
	double lat, lon;
};

struct Segment {
	Point start, end;
};


double distance(const Point& a, const Point& b) {
	return sqrt(pow(a.lat - b.lat, 2) + pow(a.lon - b.lon, 2));
}

// Function to compute the perpendicular distance from a point to a road segment
double pointToSegmentDistance(const inputPosition& p, const Segment& seg) {
	Point ip = { p.lat, p.lon };
	double x1 = seg.start.lat, y1 = seg.start.lon;
	double x2 = seg.end.lat, y2 = seg.end.lon;
	double x0 = p.lat, y0 = p.lon;

	double dx = x2 - x1;
	double dy = y2 - y1;
	double lengthSquared = dx * dx + dy * dy;

	if (lengthSquared == 0) return distance(ip, seg.start); // If segment is a point

	// Projection formula
	double t = ((x0 - x1) * dx + (y0 - y1) * dy) / lengthSquared;
	t = std::max(0.0, std::min(1.0, t)); // Clamp t to [0,1]

	Point projection = { x1 + t * dx, y1 + t * dy };
	return distance(ip, projection);
}


bool matchPosition(const inputPosition& inputPos, RoadLoader* handler, bool writeMatchingDebugDumps)
{
	auto roadNetwork = handler->getCurrentlyLoadedWays();
	double minDist = std::numeric_limits<double>::max();
	Segment currSeg;
	RoadInfo closestRoad;

	for (const auto& road : roadNetwork)
	{
		for (size_t i = 0; i < road.nodes.size() - 1; i++)
		{
			currSeg.start = { road.nodes[i].lat(), road.nodes[i].lon() };
			currSeg.end = { road.nodes[i+1].lat(), road.nodes[i+1].lon() };
			double d = pointToSegmentDistance(inputPos, currSeg);
			if (d < minDist)
			{
				closestRoad = road;
				minDist = d;
				Tracer::log("Closest road is: " + closestRoad.name + " | dist: " + std::to_string(d), traceLevel::DEBUG);
			}
		}
	}
	//std::cout << "Closest road is: " + closestRoad.name << std::endl;
	return true;
}
