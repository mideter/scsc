#pragma once

#include <netinet/in.h>

#include <iosfwd>


class ClientAddress {
public:
	ClientAddress() = default;

private:
	friend class ClientConnection;
	friend std::ostream& operator<<(std::ostream& os, const ClientAddress& address);

	sockaddr_in address_{};
};


std::ostream& operator<<(std::ostream& os, const ClientAddress& address);
