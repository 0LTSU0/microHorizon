#include "PosReceiver.h"

bool PosReceiver::initPosReceiver(MHConfigurator& conf)
{
	m_posMode = conf.getPosMode();
	switch (m_posMode)
	{
	case UDP:
		m_udpPos = UDPPositioner(conf.getUDPPort());
		return true;
	case GPS_RECEIVER:
		//todo
		break;
	default:
		break;
	}
	return false;
}

bool PosReceiver::startPosReceiver()
{
	switch (m_posMode)
	{
	case UDP:
		m_posThread = boost::thread([this]() {
			m_udpPos.run(this);
		});
		return true;
	case GPS_RECEIVER:
		//todo
		break;
	default:
		break;
	}
	return false;
}

void PosReceiver::setNewCurrentPosition(inputPosition& newPos)
{
	m_currentPosition = newPos; //todo add mutex for updating/reading this
}

inputPosition PosReceiver::getCurrentPosition()
{
	return m_currentPosition; //todo add mutex for updating/reading this
}
