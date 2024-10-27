#define NOMINMAX //fixes compilation errors from osmium

#include "OSMProcessor.h"

#include <osmium/io/reader.hpp>
#include <osmium/io/pbf_input.hpp>
#include <osmium/io/xml_input.hpp>
#include <osmium/handler.hpp>
#include <osmium/visitor.hpp>
#include <osmium/osm/box.hpp>
#include <osmium/osm/way.hpp>
#include <osmium/index/map/flex_mem.hpp>
#include <osmium/handler/node_locations_for_ways.hpp>
#include <iostream>

//note to self, getting locations for nodes is kinda complicated, good example for that https://github.com/osmcode/libosmium/blob/cf81df16ddd2fbee6eede4e84978130419eef759/examples/osmium_road_length.cpp#L3
using index_type = osmium::index::map::FlexMem<osmium::unsigned_object_id_type, osmium::Location>; //not sure what this does
using location_handler_type = osmium::handler::NodeLocationsForWays<index_type>; //but this is needed to get valid locations on nodes

class RoadLoader : public osmium::handler::Handler {
public:
    RoadLoader(osmium::Box box) {
        m_box = box;
    }
    
    void way(const osmium::Way& way) {
        // Check if the way has the "highway" tag (indicating a road)
        const char* highway_tag = way.tags()["highway"];
        if (highway_tag) {
            // Check if any of the way's nodes are within the bounding box
            for (const auto& node : way.nodes()) {
                if (node.location().is_undefined())
                {
                    Tracer::log("Node " + std::to_string(node.ref()) + " has no valid location", traceLevel::DEBUG);
                    continue;
                }
                if (m_box.contains(node.location())) {
                    std::cout << "Inside rect --- Road ID: " << way.id()
                        << " (highway=" << highway_tag << ")\n";
                    break;
                }
            }
        }
    }

private:
    osmium::Box m_box;
};


void OSMProcessor::loadRoadsAroundLoc(inputPosition& pos)
{
    float size = 0.01; //degrees around the wanted location
    osmium::Box bounding_box(
        osmium::Location(pos.lon - size, pos.lat - size),
        osmium::Location(pos.lon + size, pos.lat + size)
    );
    
    index_type index;
    location_handler_type location_handler{ index };

    RoadLoader loader(bounding_box);
    osmium::io::Reader reader(mapPath, osmium::osm_entity_bits::node | osmium::osm_entity_bits::way);
    RoadLoader handler(bounding_box);
    osmium::apply(reader, location_handler, handler);
}


bool OSMProcessor::init(MHConfigurator& configurator)
{
	mapPath = configurator.getMapPath();
	return true;
}


bool OSMProcessor::matchNewPosition(inputPosition& inputPos)
{
	Tracer::log("matchNewPosition() processing: " + std::to_string(inputPos.lat) + "," + std::to_string(inputPos.lon), traceLevel::DEBUG);
    loadRoadsAroundLoc(inputPos);
    return true;
}