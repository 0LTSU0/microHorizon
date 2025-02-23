# microHorizon

## Getting map data
This project uses libosmium for reading osm data which afaik does not provide any easy way to only load certain part of a osm file into memory. Loading e.g. entire Finland takes ~minute on decent hardware and ends up taking just under 2GB of ram. Below steps can be used to convert country extracts into suitable size.
1. Download map data (e.g Finland https://download.geofabrik.de/europe/finland-latest.osm.pbf)
2. Download Osmosis (https://github.com/openstreetmap/osmosis/releases/latest hint: way easier to get working using linux)
3. At least for now this app only uses roads from OSM data so extract those (also discard cycle- and footways):
    - osmosis.bat --read-pbf "Z:\finland-latest.osm.pbf" --tf accept-ways highway=* --tf reject-ways highway=footway,cycleway --used-node --write-pbf "Z:\finland-latest-highwaysonly.osm.pbf"
4. Split (to be figured out, for now the map is small enough after discarding unnecessary stuff)

## Building
### Windows
1. Run CMake GUI
2. Set source and build folders
3. Configure
3.5 Boost is likely not found automatically. Set BOOST_INSTALL_DIR in CMake GUI in case of configuration error and reconfigure
4. Open visual studio solution and build

### Linux
1. mkdir build && cd build
2. cmake ..
3. make -j8

### Arm crosscompile for aarch64 (raspberry pi)
1. TODO (not sure how to handle boost for cross compiling since its currently not built as part of the project)