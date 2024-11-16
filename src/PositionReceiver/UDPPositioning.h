#pragma once


class UDPPositioner
{
public:
	UDPPositioner(int port) : m_port(port) {};

private:
	int m_port;
	int m_host;
};