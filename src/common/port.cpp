#include "port.h"

#include <arpa/inet.h>

#include <ostream>
#include <stdexcept>
#include <string>


Port::Port(int port)
{
	if (port < 0 || port > 65535)
		throw std::invalid_argument("invalid port value: " + std::to_string(port));

	port_ = static_cast<uint16_t>(port);
	network_order_ = htons(port_);
}


std::ostream& operator<<(std::ostream& os, const Port& port)
{
	return os << port.port_;
}
