#include "OSMProcessor.h"
#include "PosMatcherImpl.cpp"
#include <iostream>

void RoadLoader::way(const osmium::Way& way) {
   // For some reason in OSM data "highway" means any kind of road from footpath to motorway
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
               m_currentlyLoadedWaysTMP.push_back(road_info);
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


void OSMProcessor::loadRoadsAroundLoc(inputPosition& pos)
{
    setOSMProcessorState(OSMProcessorState::ROAD_LOADING_IN_PROGRESS);
    osmium::Box bounding_box(
        osmium::Location(pos.lon - loadSizeAroundPos, pos.lat - loadSizeAroundPos),
        osmium::Location(pos.lon + loadSizeAroundPos, pos.lat + loadSizeAroundPos)
    );
    
    index_type index;
    location_handler_type location_handler{ index };

    //RoadLoader loader(bounding_box);
    osmium::io::Reader reader(mapPath, osmium::osm_entity_bits::node | osmium::osm_entity_bits::way);
    m_roadLoaderHandler.setBoudningBox(bounding_box);
    m_roadLoaderHandler.resetTMPRoadData();
    osmium::apply(reader, location_handler, m_roadLoaderHandler);
    m_roadLoaderHandler.swapLoadedData();
    setOSMProcessorState(OSMProcessorState::IDLE);
}


bool OSMProcessor::init(MHConfigurator& configurator)
{
	mapPath = configurator.getMapPath();
	return true;
}


bool OSMProcessor::matchNewPosition(inputPosition& inputPos)
{
	Tracer::log("matchNewPosition() processing: " + std::to_string(inputPos.lat) + "," + std::to_string(inputPos.lon), traceLevel::DEBUG);
    auto procState = getOSMProcessorState();
    if (procState == OSMProcessorState::NOT_INITIALIZED || procState == OSMProcessorState::LOADED_ROADS_UPDATE_NEEDED) // TODO: set state=LOADED_ROADS_UPDATE_NEEDED when needed
    {
        Tracer::log("Triggering loadRoadsAroundLoc(), reason: " + std::to_string(procState), traceLevel::INFO);
        startLoadRoadsThread(inputPos);
    }

    // state should only be NOT_INITIALIZED upon first input position -> wait for loader thread to finish before proceeding
    // otherwise there should already be loaded data than can be used until roadLoader swaps it's m_currentlyLoadedWays
    if (procState == OSMProcessorState::NOT_INITIALIZED)
    {
        waitForRoadLoaderThreadToFinish();
    }

    bool matchSuccess = matchPosition(inputPos, &m_roadLoaderHandler);
    if (!matchSuccess)
    {
        Tracer::log("Failed to match position " + inputPos.getInputPosString().str(), traceLevel::WARNING);
        if (getOSMProcessorState() != OSMProcessorState::ROAD_LOADING_IN_PROGRESS)
        {
            setOSMProcessorState(OSMProcessorState::LOADED_ROADS_UPDATE_NEEDED); //If pos match failed and map loading is not in progress then we probably have somehow ended up outside currently lodaded area
        }
    }
    return matchSuccess;
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