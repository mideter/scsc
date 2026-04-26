#include "serveraddress.h"

#include <arpa/inet.h>


ServerAddress::ServerAddress(IPv4Address ipv4, Port port)
	: ip_(ipv4), port_(port)
{}


ServerAddress ServerAddress::any(Port port)
{
	return ServerAddress(IPv4Address("0.0.0.0"), port);
}


sockaddr_in ServerAddress::value() const noexcept
{
	sockaddr_in address{};
	address.sin_family = AF_INET;
	address.sin_addr = ip_.address_;
	address.sin_port = port_.network_order();
	return address;
}
