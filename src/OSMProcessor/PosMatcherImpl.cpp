#include "OSMProcessor.h"
#include <cmath>
#include <numbers>
#include "tracer.h"
#include <boost/geometry/geometry.hpp>
#include <boost/geometry/geometries/point.hpp>
#include <boost/geometry/geometries/segment.hpp>

// TODO: not sure if using boost geometry lib causes a lot of overhead but since everything is quite simple here, 
// could possible make sense to reimplement without boost

// For conveniance
typedef boost::geometry::model::point<double, 2, boost::geometry::cs::cartesian> Point;
typedef boost::geometry::model::segment<Point> Segment;

double toRadians(double degree) {
	return degree * (std::numbers::pi / 180.0);
}

double haversineDistance(double lat1, double lon1, double lat2, double lon2) { //from chatGPT
	const double R = 6371000; // Earth's radius in meters

	double lat1_rad = toRadians(lat1);
	double lon1_rad = toRadians(lon1);
	double lat2_rad = toRadians(lat2);
	double lon2_rad = toRadians(lon2);

	double dlat = lat2_rad - lat1_rad;
	double dlon = lon2_rad - lon1_rad;

	double a = std::sin(dlat / 2) * std::sin(dlat / 2) +
		std::cos(lat1_rad) * std::cos(lat2_rad) *
		std::sin(dlon / 2) * std::sin(dlon / 2);
	double c = 2 * std::atan2(std::sqrt(a), std::sqrt(1 - a));

	return R * c; // Distance in meters
}

// TODO: come up with better logic for determening if some road is reasonable choise
// Check if node is less than 300m away from input position (using haversine)
bool checkIfSegmentIsRealisticChoise(inputPosition& inputPos, Point& testLoc)
{
	const double R = 6371.0; // Radius of the Earth in kilometers

	double testLat = boost::geometry::get<0>(testLoc);
	double testLon = boost::geometry::get<1>(testLoc);
	const double dLat = toRadians(testLat - inputPos.lat);
	const double dLon = toRadians(testLon - inputPos.lon);
	const double cLat1 = toRadians(inputPos.lat);
	const double cLat2 = toRadians(testLat);

	const double a = pow(sin(dLat / 2), 2) + cos(cLat1) * cos(cLat2) * pow(sin(dLon / 2), 2);
	const auto c = 2 * atan2(sqrt(a), sqrt(1 - a));

	double dist = R * c;
	return dist < 0.3;
}


bool matchPosition(inputPosition& inputPos, RoadLoader *handler)
{
	RoadInfo nearestRoad;
	double nearestDistance = 99999999;
	Point inputPosPoint(inputPos.lat, inputPos.lon);
	Tracer::log("Starting matchPosition(). Input pos: " + inputPos.getInputPosString().str(), traceLevel::INFO);
	bool matchSuccessfull = false; //Doesn't really tell if it actually was successfull but if true then there was something in 300m range

	//this now just finds the closest road based on which has segment closest to input pos
	//should instead find couple candidates and from those choose the most probable one (heading etc.)
	for (RoadInfo road : handler->getCurrentlyLoadedWays())
	{
		if (road.nodes.size() == 0)
		{
			continue;
		}
		else if (road.nodes.size() == 1)
		{
			Point roadNode(road.nodes.at(0).lat(), road.nodes[0].lon());
			if (!checkIfSegmentIsRealisticChoise(inputPos, roadNode))
			{
				continue; //this single node was too far away -> continue
			}
			double distance = boost::geometry::distance(inputPosPoint, roadNode);
			if (distance < nearestDistance)
			{
				nearestDistance = distance;
				nearestRoad = road;
				matchSuccessfull = true;
				if (road.id != nearestRoad.id)
				{
					Tracer::log("Nearest road to target position is now: " + nearestRoad.name + " id: " + std::to_string(nearestRoad.id), traceLevel::DEBUG);
				}
			}
		}
		else
		{
			for (size_t i = 0; i < road.nodes.size() - 1; i++)
			{
				Point segPoint1(road.nodes.at(i).lat(), road.nodes.at(i).lon());
				Point segPoint2(road.nodes.at(i+1).lat(), road.nodes.at(i+1).lon());
				if (!checkIfSegmentIsRealisticChoise(inputPos, segPoint1) || !checkIfSegmentIsRealisticChoise(inputPos, segPoint2))
				{
					continue; //at least this node in this road is too far away -> check next segment
				}
				Segment seg(segPoint1, segPoint2);
				Segment resSeg;
				boost::geometry::closest_points(inputPosPoint, seg, resSeg); //resSeg.second is the point on segement that is closest to current input position
				double haversineDist = haversineDistance(boost::geometry::get<0>(resSeg.first), boost::geometry::get<1>(resSeg.first), boost::geometry::get<0>(resSeg.second), boost::geometry::get<1>(resSeg.second));
				if (haversineDist < nearestDistance)
				{
					nearestDistance = haversineDist;
					nearestRoad = road;
					matchSuccessfull = true;
					Tracer::log("Nearest road to target position is now: " + nearestRoad.name + " id: " + std::to_string(nearestRoad.id), traceLevel::DEBUG);
				}
			}
		}

	}
	return matchSuccessfull;
}