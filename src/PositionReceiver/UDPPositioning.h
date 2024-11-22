#pragma once

#include "boost/thread.hpp"

class PosReceiver; // Forward declaration for PosReceiver that creates instance of this

class UDPPositioner
{
public:
	UDPPositioner(int port) : m_port(port) {};
	void run(PosReceiver* parent);

private:
	int m_port;
	int m_host;
	PosReceiver* m_parentPosReceiver;
};