#pragma once

#include "tracer.h"
#include "configurator.h"
#include "UDPPositioning.h"
#include "posInput.h"

#include "boost/thread.hpp"



class PosReceiver
{
public:
	bool initPosReceiver(MHConfigurator&);
	bool startPosReceiver();
	void setNewCurrentPosition(inputPosition&);
	inputPosition getCurrentPosition();

private:
	POSMode m_posMode;
	UDPPositioner m_udpPos = UDPPositioner(0);
	boost::thread m_posThread;
	inputPosition m_currentPosition;
};