#include "clientaddress.h"

#include <arpa/inet.h>

#include <stdexcept>


sockaddr* ClientAddress::sockaddr_ptr()
{
	return reinterpret_cast<sockaddr*>(&address_);
}


socklen_t ClientAddress::sockaddr_size() const noexcept
{
	return sizeof(address_);
}


std::string ClientAddress::ip_string() const
{
	char ip[INET_ADDRSTRLEN] = {0};
	if (::inet_ntop(AF_INET, &address_.sin_addr, ip, sizeof(ip)) == nullptr)
		throw std::runtime_error("failed to format client ip");

	return std::string(ip);
}


uint16_t ClientAddress::port() const noexcept
{
	return ntohs(address_.sin_port);
}
