#include "serveraddress.h"

#include <arpa/inet.h>


ServerAddress::ServerAddress(IPv4Address ipv4, Port port)
	: ip_(ipv4), port_(port)
{
	address_.sin_family = AF_INET;
	address_.sin_addr = ip_.address_;
	address_.sin_port = port_.network_order();
}


ServerAddress ServerAddress::any(Port port)
{
	return ServerAddress(IPv4Address("0.0.0.0"), port);
}
