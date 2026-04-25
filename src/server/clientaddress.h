#pragma once

#include <netinet/in.h>

#include <cstdint>
#include <string>


class ClientAddress {
public:
	ClientAddress() = default;

	sockaddr* sockaddr_ptr();
	socklen_t sockaddr_size() const noexcept;

	std::string ip_string() const;
	uint16_t port() const noexcept;

private:
	sockaddr_in address_{};
};
