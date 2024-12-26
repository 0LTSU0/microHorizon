#include "RoadLoader.h"

void RoadLoader::way(const osmium::Way& way) {
    // For some reason in OSM data "highway" means any kind of road from footpath to motorway
    const char* highway_tag = way.tags()["highway"];
    if (highway_tag && std::string(highway_tag) != "path") { // TODO: check if paths need to be included.. https://wiki.openstreetmap.org/wiki/Key:highway#Paths -> probably not
        // Check if any of the way's nodes are within the bounding box, if yes make roadInfo obj and add to m_currentlyLoadedWaysTMP
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

                RoadInfo roadInfo{
                     way.id(),
                     way.tags().get_value_by_key("name", "Unnamed Road"),
                     highway_tag,
                     road_nodes
                };
                m_currentlyLoadedWaysTMP.push_back(roadInfo);
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
