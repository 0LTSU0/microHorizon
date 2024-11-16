#pragma once

#include "tracer.h"

#define NOMINMAX //fixes compilation errors from osmium
#include <osmium/io/reader.hpp>
#include <osmium/io/pbf_input.hpp>
#include <osmium/io/xml_input.hpp>
#include <osmium/handler.hpp>
#include <osmium/visitor.hpp>
#include <osmium/osm/box.hpp>
#include <osmium/osm/way.hpp>
#include <osmium/index/map/flex_mem.hpp>
#include <osmium/handler/node_locations_for_ways.hpp>

//note to self, getting locations for nodes is kinda complicated, good example for that https://github.com/osmcode/libosmium/blob/cf81df16ddd2fbee6eede4e84978130419eef759/examples/osmium_road_length.cpp#L3
using index_type = osmium::index::map::FlexMem<osmium::unsigned_object_id_type, osmium::Location>; //not sure what this does
using location_handler_type = osmium::handler::NodeLocationsForWays<index_type>; //but this is needed to get valid locations on nodes

struct RoadInfo {
    osmium::object_id_type id;
    std::string name;
    std::string highway_type;
    std::vector<osmium::Location> nodes;
};

class RoadLoader : public osmium::handler::Handler {
public:
    void way(const osmium::Way& way);
    void setBoudningBox(osmium::Box& box);
    osmium::Box getCurrentBoundingBox();

    std::vector<RoadInfo> getCurrentlyLoadedWays() {
        std::lock_guard<std::mutex> lock(roadInfoMtx);
        return m_currentlyLoadedWays;
    }

    void swapLoadedData() //After map loading is done, this needs to be called to make the new data available to OSMProcessor
    {
        std::lock_guard<std::mutex> lock(roadInfoMtx);
        m_currentlyLoadedWays = m_currentlyLoadedWaysTMP;
    }

    void resetTMPRoadData() //Before starting new map loading, this needs to be called to clear tmp data
    {
        m_currentlyLoadedWaysTMP.clear();
    }

private:
    osmium::Box m_box;
    std::vector<RoadInfo> m_currentlyLoadedWays; //this is the loaded data that OSMProcessor is using for pos matching
    std::vector<RoadInfo> m_currentlyLoadedWaysTMP; //this for internal use. way() adds loaded roads to it. When loading is finished, swapLoadedData() needs to be called
    std::mutex roadInfoMtx; //to make sure getCurrentlyLoadedWays() doesnt read data when swapLoadedData is updating it
};
