#include <iostream>
#include "tracer.h"
#include "OSMProcessor.h"
#include "configurator.h"

OSMProcessor osmProcessor;
MHConfigurator appConfigurator;

int main()
{
	Tracer::log("Starting michroHorizonApp", traceLevel::INFO);

	appConfigurator.loadConfig();
	
	inputPosition pos;
	pos.lat = 65.061673;
	pos.lon = 25.504593;

	osmProcessor.init(appConfigurator);
	osmProcessor.matchNewPosition(pos);

	std::cout << "exiting()" << std::endl;
}