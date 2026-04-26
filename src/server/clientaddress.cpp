#include "clientaddress.h"

#include <ostream>


namespace {


uint16_t port(const sockaddr_in& address)
{
	return ntohs(address.sin_port);
}


}  // namespace


ClientAddress::ClientAddress(sockaddr_in addr)
	: ip_(IPv4(addr.sin_addr))
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
