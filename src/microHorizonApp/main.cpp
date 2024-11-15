#include <iostream>
#include "tracer.h"
#include "OSMProcessor.h"
#include "configurator.h"

#include <boost/thread.hpp>

OSMProcessor osmProcessor;
MHConfigurator appConfigurator;

int main()
{
	Tracer::log("Starting michroHorizonApp", tracer::traceLevel::INFO);

	if (!appConfigurator.loadConfig())
	{
		Tracer::log("loadConfig() failed, cannot start application", tracer::traceLevel::ERROR);
		return -1;
	}
	

	//someTestFunc();
	std::cout << "exiting()" << std::endl;
}


// function to call in main() when need debug
void someTestFunc()
{
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

	pos.lat = 60.211010; // somewhere far away (triggers map load needed and fails to match)
	pos.lon = 24.900657;
	osmProcessor.matchNewPosition(pos);

	pos.lat = 60.211827; // probably still fails since map loading takes a while
	pos.lon = 24.901623;
	osmProcessor.matchNewPosition(pos);

	//boost::this_thread::sleep_for(boost::chrono::milliseconds(5000));
	osmProcessor.waitForRoadLoaderThreadToFinish();
	pos.lat = 60.211827; // same pos as previously, should match since we waited for laoder to finish to Hämeenlinnanväylä
	pos.lon = 24.901623;
	osmProcessor.matchNewPosition(pos);
}