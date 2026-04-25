#pragma once

#include <netinet/in.h>

#include <string>


class IPv4Address {
public:
	explicit IPv4Address(std::string address);

	const in_addr& value() const noexcept;

private:
	in_addr address_{};
};
