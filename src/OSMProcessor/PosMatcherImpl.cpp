#include "OSMProcessor.h"
#include <cmath>
#include "tracer.h"





bool matchPosition(inputPosition& inputPos, RoadLoader *handler)
{
	RoadInfo nearestRoad;
	double nearestDistance = 99999999;
	//this now just finds the closest road based on which has segment closest to input pos
	//should instead find couple candidates and from those choose the most probable one (heading etc.)
	for (RoadInfo road : handler->getCurrentlyLoadedWays())
	{
		for (size_t i = 0; i < road.nodes.size(); i++)
		{
			double segmentDistance = distanceToSegmentSquared(inputPos.lat, inputPos.lon,
				road.nodes[i - 1].lat(), road.nodes[i - 1].lon(),
				road.nodes[i].lat(), road.nodes[i].lon());
			if (segmentDistance < nearestDistance)
			{
				nearestDistance = segmentDistance;
				nearestRoad = road;
				Tracer::log("Nearest road to target position is now: " + nearestRoad.name + " id: " + std::to_string(nearestRoad.id), traceLevel::DEBUG);
				break;
			}
		}
		
		
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