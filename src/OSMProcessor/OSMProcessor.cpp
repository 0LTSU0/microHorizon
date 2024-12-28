#include "OSMProcessor.h"
#include "PosMatcherImpl.cpp"
#include <iostream>


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
    loadSizeAroundPos = configurator.getLoadRadius();
	return true;
}


//TODO: There is currently a risk that startLoadRoadsThread() gets called tiwice if this function is called in quick succession since 
//loadRoadsAroundLoc() thread might not have had time to change state to ROAD_LOADING_IN_PROGRESS by the time it gets triggered again
//-> should redo this. It also doesnt start load on failed match now but only on the next position -> that will likely fail too
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
        Tracer::log("Failed to match position " + inputPos.getInputPosString().str() + " settings state: LOADED_ROADS_UPDATE_NEEDED", traceLevel::WARNING);
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