#pragma once

#include <netinet/in.h>

#include <string>


class IPv4Address {
public:
	explicit IPv4Address(std::string address);

private:
	friend class ServerAddress;

	in_addr address_{};
};
