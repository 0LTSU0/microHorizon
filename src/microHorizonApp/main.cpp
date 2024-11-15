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
	pos.lat = 65.061673; //alakyläntie
	pos.lon = 25.504593;

	osmProcessor.init(appConfigurator);
	osmProcessor.matchNewPosition(pos);
	
	pos.lat = 65.064451; // kerttulantie
	pos.lon = 25.504145;
	osmProcessor.matchNewPosition(pos);

	pos.lat = 65.064229; // raitotie
	pos.lon = 25.508297;
	osmProcessor.matchNewPosition(pos);

	std::cout << "exiting()" << std::endl;
}