//#include "OSMProcessor.h"
#include "PosMatcherImpl.h"
#include <cmath>
#include <numbers>
#include <set>
#include <fstream>
#include <limits>
#include "tracer.h"
#include "json.hpp"
#include "iostream"
#include <boost/thread.hpp>

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


void findClosestRoadFromSet(const inputPosition& inputPos, const std::vector<RoadInfo>& roadNetwork, int start_idx, int end_idx, RoadInfo& RoadRes, double& DistRes, int thread)
{
	double minDist = std::numeric_limits<double>::max();
	Segment currSeg;
	for (int i = start_idx; i < end_idx; i++)
	{
		const auto road = roadNetwork[i];
		for (size_t i = 0; i < road.nodes.size() - 1; i++)
		{
			currSeg.start = { road.nodes[i].lat(), road.nodes[i].lon() };
			currSeg.end = { road.nodes[i + 1].lat(), road.nodes[i + 1].lon() };
			double d = pointToSegmentDistance(inputPos, currSeg);
			if (d < minDist)
			{
				minDist = d;
				RoadRes = road;
				DistRes = minDist;
				Tracer::log("Closest road is: " + RoadRes.name + " | dist: " + std::to_string(d) + " | th: " + std::to_string(thread), traceLevel::DEBUG);
			}
		}
	}
}


bool matchPosition(const inputPosition& inputPos, RoadLoader* handler, bool writeMatchingDebugDumps)
{
	auto roadNetwork = handler->getCurrentlyLoadedWays();
	RoadInfo closestRoad;
	double nearestDistance;

	auto start = std::chrono::high_resolution_clock::now();
	// When we have largeish map loaded into memory, multithread this (NVM it dont work)
	if (roadNetwork.size() > 50000 && false)
	{
		int numItemsPerThread = static_cast<int>(roadNetwork.size() / 2);
		RoadInfo closestRoad1, closestRoad2;
		double nearestDistance1 = 0.0;
		double nearestDistance2 = 0.0;
		boost::thread thread1(findClosestRoadFromSet, inputPos, roadNetwork, 0, numItemsPerThread, closestRoad1, nearestDistance1, 0);
		boost::thread thread2(findClosestRoadFromSet, inputPos, roadNetwork, numItemsPerThread, roadNetwork.size(), closestRoad2, nearestDistance2, 1);
		thread1.join();
		thread2.join();
		if (nearestDistance1 < nearestDistance2)
		{
			nearestDistance = nearestDistance1;
			closestRoad = closestRoad1;
		}
		else {
			nearestDistance = nearestDistance2;
			closestRoad = closestRoad2;
		}
	}
	else {
		findClosestRoadFromSet(inputPos, roadNetwork, 0, roadNetwork.size(), closestRoad, nearestDistance, 0);
	}
	std::chrono::duration<double, std::milli> duration = std::chrono::high_resolution_clock::now() - start;
	Tracer::log("Closest road is: " + closestRoad.name + " | dist: " + std::to_string(nearestDistance) + " | took: " + std::to_string(duration.count()) + "ms", traceLevel::DEBUG);
	return true;
}
