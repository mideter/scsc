#include "ipv4address.h"

#include <arpa/inet.h>

#include <cerrno>
#include <stdexcept>
#include <system_error>


IPv4Address::IPv4Address(std::string address)
	: address_{}
{
	const int result = ::inet_pton(AF_INET, address.c_str(), &address_);

	if (result == -1)
		throw std::system_error(errno, std::generic_category(), "inet_pton failed");

	if (result == 0)
		throw std::invalid_argument("invalid IPv4 address format: " + address);
}
