#pragma once

#include <netinet/in.h>

#include <cstdint>
#include <iosfwd>


class Port {
public:
	explicit Port(int port);

private:
	friend class ServerAddress;
	friend std::ostream& operator<<(std::ostream& os, const Port& port);

	in_port_t network_order() const noexcept;

	uint16_t port_;
};


std::ostream& operator<<(std::ostream& os, const Port& port);
