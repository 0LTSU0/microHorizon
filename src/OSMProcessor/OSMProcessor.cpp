#define NOMINMAX //fixes compilation errors from osmium

#include "OSMProcessor.h"

#include <osmium/io/reader.hpp>
#include <osmium/io/pbf_input.hpp>
#include <osmium/io/xml_input.hpp>
#include <osmium/handler.hpp>
#include <osmium/visitor.hpp>
#include <osmium/osm/box.hpp>
#include <osmium/osm/way.hpp>
#include <iostream>

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
            std::cout << way.id() << std::endl;
            for (const auto& node : way.nodes()) {
                std::cout << node.ref() << std::endl;
                if (node.location().is_defined())
                {
                    std::cout << node.ref() << std::endl;
                }
                
                //if (m_box.contains(node.location())) {
                //    std::cout << "Road ID: " << way.id()
                //        << " (highway=" << highway_tag << ")\n";
                //    break;
                //}
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
    RoadLoader loader(bounding_box);
    osmium::io::Reader reader(mapPath, osmium::osm_entity_bits::node | osmium::osm_entity_bits::way);
    RoadLoader handler(bounding_box);
    osmium::apply(reader, handler);
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