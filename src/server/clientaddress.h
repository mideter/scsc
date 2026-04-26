#pragma once

#include <netinet/in.h>
#include <iosfwd>

#include "ipv4.h"
#include "port.h"


class ClientAddress {
public:
	ClientAddress(const ClientAddress&) = default;
	ClientAddress& operator=(const ClientAddress&) = default;
	ClientAddress(ClientAddress&&) = default;
	ClientAddress& operator=(ClientAddress&&) = default;

private:
	friend class ClientConnection;

	explicit ClientAddress(sockaddr_in addr);

	friend std::ostream& operator<<(std::ostream& os, const ClientAddress& address);

	IPv4 ip_;
	Port port_;

	sockaddr_in address_{};
};


std::ostream& operator<<(std::ostream& os, const ClientAddress& address);
