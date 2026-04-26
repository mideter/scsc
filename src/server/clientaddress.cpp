#include "clientaddress.h"

#include <arpa/inet.h>

#include <ostream>
#include <stdexcept>


namespace {


std::string ip_string(const sockaddr_in& address)
{
	char ip[INET_ADDRSTRLEN] = {0};
	if (::inet_ntop(AF_INET, &address.sin_addr, ip, sizeof(ip)) == nullptr)
		throw std::runtime_error("failed to format client ip");

	return std::string(ip);
}


uint16_t port(const sockaddr_in& address)
{
	return ntohs(address.sin_port);
}


}  // namespace


ClientAddress::ClientAddress(sockaddr_in addr)
	: ip_(ip_string(addr))
	, port_(static_cast<int>(port(addr)))
	, address_(addr)
{}


ClientAddress ClientAddress::from_sockaddr_in(sockaddr_in addr)
{
	return ClientAddress(addr);
}


std::ostream& operator<<(std::ostream& os, const ClientAddress& address)
{
	return os << address.ip_ << ":" << address.port_;
}
