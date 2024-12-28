#include "tracer.h"
#include "OSMProcessor.h"
#include "configurator.h"
#include "PosReceiver.h"
#include "posInput.h"

#include <iostream>
#include <boost/thread.hpp>

#undef ERROR // Some header defines some ERROR that conflicts with traceLevel::ERROR on windows -> workaround

OSMProcessor osmProcessor;
MHConfigurator appConfigurator;
PosReceiver positionReceiver;

// temp function to debug stuff
static void someTestFunc()
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


void udpWorkLoop(PosReceiver &posReceiver)
{
	inputPosition prevPos;
	while (true)
	{
		inputPosition currPos = positionReceiver.getCurrentPosition();
		if (currPos != prevPos)
		{
			osmProcessor.matchNewPosition(currPos);
			prevPos = currPos;
		}
		boost::this_thread::sleep_for(boost::chrono::milliseconds(100));
	}
}


cmdArgs parseCMDArgs(int argc, char* argv[])
{
	cmdArgs args;
	std::string currArg;
	std::string searchStr;
	for (int i = 0; i < argc; i++)
	{
		currArg = argv[i];
		
		searchStr = "cfg=";
		auto findPos = currArg.find(searchStr);
		if (findPos != std::string::npos)
		{
			args.configPath = currArg.substr(findPos + searchStr.length());
		}
		searchStr = "someOtherArg"; // TODO: other args if necessary
	}
	return args;
}


int main(int argc, char* argv[])
{
	Tracer::log("Starting michroHorizonApp", traceLevel::INFO);

	cmdArgs args = parseCMDArgs(argc, argv);

	if (!appConfigurator.loadConfig(args.configPath))
	{
		Tracer::log("loadConfig() failed, cannot start application", traceLevel::ERROR);
		return -1;
	}

	osmProcessor.init(appConfigurator);
	positionReceiver.initPosReceiver(appConfigurator);
	positionReceiver.startPosReceiver();

	//someTestFunc();
	//int ctr = 0;
	//while (ctr < 10)
	//{
	//	boost::this_thread::sleep_for(boost::chrono::milliseconds(1000));
	//	inputPosition inPos = positionReceiver.getCurrentPosition();
	//	osmProcessor.matchNewPosition(inPos);
	//	ctr++;
	//}
	udpWorkLoop(positionReceiver);
	
	std::cout << "exiting()" << std::endl;
}


