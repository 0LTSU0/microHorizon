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
	std::lock_guard<std::mutex> lock(posLock);
	m_currentPosition = newPos;
}

inputPosition PosReceiver::getCurrentPosition()
{
	std::lock_guard<std::mutex> lock(posLock);
	return m_currentPosition;
}
