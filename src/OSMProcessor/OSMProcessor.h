#pragma once

#include "tracer.h"
#include "configurator.h"
#include "RoadLoader.h"
#include "posInput.h"

#include <atomic>
#include <mutex>
#include <boost/thread.hpp>


//this is actually more of a rodaloader status but whatever
enum OSMProcessorState
{
    NOT_INITIALIZED,
    IDLE,
    LOADED_ROADS_UPDATE_NEEDED,
    ROAD_LOADING_IN_PROGRESS
};

class OSMProcessor {
public:
	bool init(MHConfigurator&);
	bool matchNewPosition(inputPosition&);
	void loadRoadsAroundLoc(inputPosition& pos);

	RoadLoader m_roadLoaderHandler = RoadLoader();
    float loadSizeAroundPos = 0.01; //degrees around the wanted location that shall be loaded when loadRoadsAroundLoc() is called

    void setOSMProcessorState(OSMProcessorState newState)
    {
        std::lock_guard<std::mutex> lock(stateLock);
        m_state = newState;
    }

    OSMProcessorState getOSMProcessorState()
    {
        std::lock_guard<std::mutex> lock(stateLock);
        return m_state;
    }

    void startLoadRoadsThread(inputPosition& pos) {
        roadLoaderThread = boost::thread(&OSMProcessor::loadRoadsAroundLoc, this, std::ref(pos));
    }

    void waitForRoadLoaderThreadToFinish() {
        while (getOSMProcessorState() == OSMProcessorState::NOT_INITIALIZED || getOSMProcessorState() == OSMProcessorState::ROAD_LOADING_IN_PROGRESS) {
            Tracer::log("Waiting for roadLoaderThread to finish. Current sate: " + std::to_string(getOSMProcessorState()), traceLevel::DEBUG);
            boost::this_thread::sleep_for(boost::chrono::milliseconds(100));
        }
    }

private:
	std::string mapPath = "";
    OSMProcessorState m_state = OSMProcessorState::NOT_INITIALIZED;
    std::mutex stateLock;
    boost::thread roadLoaderThread;
};