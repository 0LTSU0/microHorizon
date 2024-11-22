#include "UDPPositioning.h"
#include "posInput.h"
#include "PosReceiver.h"
#include <iostream>

static double randomFloat() //temp test
{
	return (double)(rand()) / (double)(rand());
}

void UDPPositioner::run(PosReceiver* parent)
{
	m_parentPosReceiver = parent;
	while (true)
	{
		boost::this_thread::sleep_for(boost::chrono::milliseconds(100));
		std::cout << "this is printed from UDPPositioner::run" << std::endl;
		inputPosition newPos;
		newPos.lat = randomFloat();
		newPos.lon = randomFloat();
		newPos.heading = randomFloat();
		m_parentPosReceiver->setNewCurrentPosition(newPos);
	}
	
}