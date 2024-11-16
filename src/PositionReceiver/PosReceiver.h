#pragma once

#include "tracer.h"
#include "configurator.h"
#include "UDPPositioning.h"

#include <boost/thread.hpp>

class PosReceiver
{
	bool initPosReceiver(MHConfigurator&);

private:
	POSMode m_posMode;
	UDPPositioner m_udpPos;
};