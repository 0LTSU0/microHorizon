add_library(PositionReceiver PosReceiver.cpp
							 PosReceiver.h
							 UDPPositioning.cpp
							 UDPPositioning.h)
							 
target_link_libraries(PositionReceiver PUBLIC Tracer ${Boost_LIBRARIES})

target_include_directories(PositionReceiver PUBLIC 
						   "${CMAKE_CURRENT_SOURCE_DIR}"
						   "${CMAKE_CURRENT_SOURCE_DIR}/../microHorizonApp" #for configurator
						   "${CMAKE_CURRENT_SOURCE_DIR}/../../ThirdParty/nlohmann_json-3.11.3" #configurator needs json
						   "${Boost_INCLUDE_DIRS}")