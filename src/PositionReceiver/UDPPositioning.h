#pragma once

#include "tracer.h"
#include "posInput.h"

#include "boost/thread.hpp"
#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <boost/bind.hpp>

// Format for udp data is string in format: "lat,lon,speed,heading"

class PosReceiver; // Forward declaration for PosReceiver that creates instance of this

class UDPPositioner
{
public:
	UDPPositioner(int port) : m_port(port) {};
	void run(PosReceiver* parent);

private:
	int m_port;
	PosReceiver* m_parentPosReceiver;
};