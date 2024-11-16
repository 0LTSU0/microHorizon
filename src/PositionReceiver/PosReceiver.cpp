#include "PosReceiver.h"

bool PosReceiver::initPosReceiver(MHConfigurator& conf)
{
	m_posMode = conf.getPosMode();
	switch (m_posMode)
	{
	case UDP:
		m_udpPos = UDPPositioner(conf.getUDPPort());
		break;
	case GPS_RECEIVER:
		//todo
		break;
	default:
		break;
	}
}