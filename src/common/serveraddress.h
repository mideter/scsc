#pragma once

#include <netinet/in.h>

#include "ipv4.h"
#include "port.h"


class ServerAddress {
public:
	ServerAddress(IPv4 ipv4, Port port);
	
	static ServerAddress any(Port port);

private:
	friend class EchoServer;
	friend class EchoClient; 

	IPv4 ip_;
	Port port_;

	sockaddr_in address_{};
};
