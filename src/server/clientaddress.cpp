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


sockaddr* ClientAddress::sockaddr_ptr()
{
	return reinterpret_cast<sockaddr*>(&address_);
}


socklen_t ClientAddress::sockaddr_size() const noexcept
{
	return sizeof(address_);
}


std::ostream& operator<<(std::ostream& os, const ClientAddress& address)
{
	return os << ip_string(address.address_) << ":" << port(address.address_);
}
