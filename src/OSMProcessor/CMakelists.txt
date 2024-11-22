add_library(OSMProcessor OSMProcessor.cpp
						 OSMProcessor.h
						 RoadLoader.cpp
						 RoadLoader.h
						 PosMatcherImpl.cpp)

target_link_libraries(OSMProcessor PUBLIC Tracer zlibstatic expat ${Boost_LIBRARIES})

target_include_directories(OSMProcessor PUBLIC 
						   "${CMAKE_CURRENT_SOURCE_DIR}/../../ThirdParty/libosmium-2.20.0/include"
						   "${CMAKE_CURRENT_SOURCE_DIR}"
						   "${CMAKE_CURRENT_SOURCE_DIR}/../microHorizonApp" #for configurator and position input
						   "${CMAKE_CURRENT_SOURCE_DIR}/../../ThirdParty/nlohmann_json-3.11.3" #configurator needs json
						   "${CMAKE_CURRENT_SOURCE_DIR}/../../ThirdParty/protozero-1.7.1/include"
						   "${Boost_INCLUDE_DIRS}")
