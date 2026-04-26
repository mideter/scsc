#pragma once

#include <netinet/in.h>

#include <string>


class IPv4 {
public:
	explicit IPv4(std::string address);

private:
	friend class ServerAddress;

	std::string ip_;
	in_addr network_order_;
};
