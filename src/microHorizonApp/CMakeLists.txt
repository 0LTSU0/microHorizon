add_executable(microHorizonApp main.cpp
							   configurator.cpp
							   configurator.h
							   posInput.h)

target_link_libraries(microHorizonApp PUBLIC OSMProcessor Tracer PositionReceiver ${Boost_LIBRARIES})

target_include_directories(microHorizonApp PUBLIC 
							"${CMAKE_CURRENT_SOURCE_DIR}/../../ThirdParty/libosmium-2.20.0/include"
							"${CMAKE_CURRENT_SOURCE_DIR}/../../ThirdParty/nlohmann_json-3.11.3")