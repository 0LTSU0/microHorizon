#pragma once

#define NOMINMAX //fixes compilation errors from osmium

#include "tracer.h"
#include "configurator.h"

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

class inputPosition {
public:
	float lat;
	float lon;
    float heading = 0.0;
};

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
    osmium::Box RoadLoader::getCurrentBoundingBox();
    void resetLoadedData();

    const std::vector<RoadInfo>& getCurrentlyLoadedWays() const {
        return m_currentlyLoadedWays;
    }

private:
    osmium::Box m_box;
    std::vector<RoadInfo> m_currentlyLoadedWays;
};

class OSMProcessor {
public:
	bool init(MHConfigurator&);
	bool matchNewPosition(inputPosition&);
	void loadRoadsAroundLoc(inputPosition& pos);

	RoadLoader m_roadLoaderHandler = RoadLoader();
private:
	std::string mapPath = "";
};