#pragma once

#include <netinet/in.h>

#include "ipv4address.h"
#include "port.h"


class ServerAddress {
public:
	ServerAddress(IPv4Address ipv4, Port port);
	
	static ServerAddress any(Port port);

	sockaddr_in value() const noexcept;

private:
	IPv4Address ip_;
	Port port_;
};
