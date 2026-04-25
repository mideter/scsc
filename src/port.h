#pragma once

#include <netinet/in.h>

#include <cstdint>


class Port {
public:
	explicit Port(int port);

	uint16_t value() const noexcept;
	in_port_t network_order() const noexcept;

private:
	uint16_t port_;
};
