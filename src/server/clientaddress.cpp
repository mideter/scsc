#include "clientaddress.h"

#include <ostream>


ClientAddress::ClientAddress(sockaddr_in addr)
	: ip_{addr.sin_addr}
	, port_{addr.sin_port}
	, address_(addr)
{}


std::ostream& operator<<(std::ostream& os, const ClientAddress& address)
{
	return os << address.ip_ << ":" << address.port_;
}
