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
	
	std::array<char, 1024> recvBuffer;
	boost::asio::io_context io_context;
	boost::asio::ip::udp::endpoint listen_endpoint(boost::asio::ip::udp::v4(), m_port);
	boost::asio::ip::udp::socket socket(io_context, listen_endpoint);
	Tracer::log("UDPPositioner opened positioning socket to " + std::to_string(m_port), traceLevel::INFO);
	

	while (true)
	{
		boost::asio::ip::udp::endpoint sender_endpoint;
		size_t bReceived = socket.receive_from(
			boost::asio::buffer(recvBuffer), sender_endpoint);
		std::string recvStr = std::string(recvBuffer.data(), bReceived);
		//Tracer::log("UDPPositioner received from socket " + recvStr, traceLevel::DEBUG);

		std::stringstream ss(recvStr);
		std::string token;
		auto newPos = inputPosition();
		if (std::getline(ss, token, ',')) {
			newPos.lat = std::stod(token);
		}
		if (std::getline(ss, token, ',')) {
			newPos.lon = std::stod(token);
		}
		if (std::getline(ss, token, ',')) {
			newPos.speed = std::stod(token);
		}
		if (std::getline(ss, token, ',')) {
			newPos.heading = std::stod(token);
		}
		m_parentPosReceiver->setNewCurrentPosition(newPos);
	}
	
}