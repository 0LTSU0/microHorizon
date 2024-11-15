#include "OSMProcessor.h"
#include <cmath>
#include "tracer.h"
#include <boost/geometry/geometry.hpp>
#include <boost/geometry/geometries/point.hpp>
#include <boost/geometry/geometries/segment.hpp>

// TODO: not sure if using boost geometry lib causes a lot of overhead but since everything is quite simple here, 
// could possible make sense to reimplement without boost

// For conveniance
typedef boost::geometry::model::point<double, 2, boost::geometry::cs::cartesian> Point;
typedef boost::geometry::model::segment<Point> Segment;

bool matchPosition(inputPosition& inputPos, RoadLoader *handler)
{
	RoadInfo nearestRoad;
	double nearestDistance = 99999999;
	Point inputPosPoint(inputPos.lat, inputPos.lon);
	Tracer::log("Starting matchPosition(). Input pos: " + inputPos.getInputPosString().str(), traceLevel::INFO);

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
			double distance = boost::geometry::distance(inputPosPoint, roadNode);
			if (distance < nearestDistance)
			{
				nearestDistance = distance;
				nearestRoad = road;
				Tracer::log("Nearest road to target position is now: " + nearestRoad.name + " id: " + std::to_string(nearestRoad.id), traceLevel::DEBUG);
			}
		}
		else
		{
			for (size_t i = 0; i < road.nodes.size() - 1; i++)
			{
				Point segPoint1(road.nodes.at(i).lat(), road.nodes.at(i).lon());
				Point segPoint2(road.nodes.at(i+1).lat(), road.nodes.at(i+1).lon());
				Segment seg(segPoint1, segPoint2);
				double distance = boost::geometry::distance(inputPosPoint, seg);
				if (distance < nearestDistance)
				{
					nearestDistance = distance;
					nearestRoad = road;
					Tracer::log("Nearest road to target position is now: " + nearestRoad.name + " id: " + std::to_string(nearestRoad.id), traceLevel::DEBUG);
				}
			}
		}
		
		
		// old implementation that finds closest node. Does not work very well
		//for (auto node : road.nodes)
		//{
		//	double dist = std::sqrt(std::pow(inputPos.lat - node.lat(), 2) +
		//		std::pow(inputPos.lon - node.lon(), 2));
		//	if (dist < nearestDistance)
		//	{
		//		nearestDistance = dist;
		//		nearestRoad = road;
		//		Tracer::log("Nearest road to target position is now: " + nearestRoad.name + " id: " + std::to_string(nearestRoad.id), traceLevel::DEBUG);
		//		break;
		//	}
		//}
	}
	return true;
}