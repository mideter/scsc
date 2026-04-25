#include "port.h"

#include <arpa/inet.h>

#include <stdexcept>
#include <string>


Port::Port(int port)
{
	if (port < 0 || port > 65535)
		throw std::invalid_argument("invalid port value: " + std::to_string(port));

	port_ = static_cast<uint16_t>(port);
}


uint16_t Port::value() const noexcept
{
	return port_;
}


in_port_t Port::network_order() const noexcept
{
	return htons(port_);
}
