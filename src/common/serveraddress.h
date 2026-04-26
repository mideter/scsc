#pragma once

#include <netinet/in.h>

#include "ipv4address.h"
#include "port.h"


class ServerAddress {
public:
	ServerAddress(IPv4Address ipv4, Port port);
	
	static ServerAddress any(Port port);

private:
	friend class EchoServer;
	friend class EchoClient; 

	IPv4Address ip_;
	Port port_;

	sockaddr_in address_{};
};
