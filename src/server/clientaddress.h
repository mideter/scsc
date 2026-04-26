#pragma once

#include <netinet/in.h>

#include <iosfwd>


class ClientAddress {
public:
	ClientAddress() = default;

	sockaddr* sockaddr_ptr();
	socklen_t sockaddr_size() const noexcept;

private:
	friend std::ostream& operator<<(std::ostream& os, const ClientAddress& address);

	sockaddr_in address_{};
};


std::ostream& operator<<(std::ostream& os, const ClientAddress& address);
