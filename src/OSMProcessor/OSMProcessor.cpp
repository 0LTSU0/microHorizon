#include "OSMProcessor.h"
#include "PosMatcherImpl.cpp"
#include <iostream>

void RoadLoader::way(const osmium::Way& way) {
   // Check if the way has the "highway" tag (indicating a road)
   const char* highway_tag = way.tags()["highway"];
   if (highway_tag && std::string(highway_tag) != "path") { // TODO: check if paths need to be included.. https://wiki.openstreetmap.org/wiki/Key:highway#Paths -> probably not
       // Check if any of the way's nodes are within the bounding box
       for (const auto& node : way.nodes()) {
           if (node.location().is_undefined())
           {
               Tracer::log("Node " + std::to_string(node.ref()) + " has no valid location", traceLevel::DEBUG);
               continue;
           }
           if (m_box.contains(node.location())) {
               Tracer::log("Inside rect --- Road ID: " + std::to_string(way.id()) + " (highway=" + highway_tag + ")", traceLevel::DEBUG);
               std::vector<osmium::Location> road_nodes;
               for (const auto& node : way.nodes()) {
                   if (node.location().is_undefined()) {
                       // in case this way had some undefined node location
                       continue;
                   }
                   road_nodes.push_back(node.location());
               }

               RoadInfo road_info{
                    way.id(),
                    way.tags().get_value_by_key("name", "Unnamed Road"),
                    highway_tag,
                    road_nodes
               };
               m_currentlyLoadedWays.push_back(road_info);
               break;
           }
       }
    }
}

void RoadLoader::setBoudningBox(osmium::Box& box)
{
    m_box = box;
}

osmium::Box RoadLoader::getCurrentBoundingBox()
{
    return m_box;
}

void RoadLoader::resetLoadedData()
{
    m_currentlyLoadedWays.clear();
}


void OSMProcessor::loadRoadsAroundLoc(inputPosition& pos)
{
    float size = 0.01; //degrees around the wanted location
    osmium::Box bounding_box(
        osmium::Location(pos.lon - size, pos.lat - size),
        osmium::Location(pos.lon + size, pos.lat + size)
    );
    
    index_type index;
    location_handler_type location_handler{ index };

    //RoadLoader loader(bounding_box);
    m_roadLoaderHandler.resetLoadedData();
    osmium::io::Reader reader(mapPath, osmium::osm_entity_bits::node | osmium::osm_entity_bits::way);
    m_roadLoaderHandler.setBoudningBox(bounding_box);
    osmium::apply(reader, location_handler, m_roadLoaderHandler);
}


bool OSMProcessor::init(MHConfigurator& configurator)
{
	mapPath = configurator.getMapPath();
	return true;
}


bool OSMProcessor::matchNewPosition(inputPosition& inputPos)
{
	Tracer::log("matchNewPosition() processing: " + std::to_string(inputPos.lat) + "," + std::to_string(inputPos.lon), traceLevel::DEBUG);
    loadRoadsAroundLoc(inputPos); // TODO: only do this if input pos is too close to endge of currently loaded area
    bool matchSuccess = matchPosition(inputPos, &m_roadLoaderHandler);
    return true;
}


std::stringstream inputPosition::getInputPosString()
{
    std::stringstream ss;
    ss << std::to_string(lat);
    ss << ",";
    ss << std::to_string(lon);
    ss << ",";
    ss << std::to_string(heading);
    return ss;
}