#pragma once

#include <cstddef>
#include <string>
#include <string_view>

#include "serveraddress.h"
#include "sockethandle.h"


class MessengerClient {
public:
	MessengerClient();

	void connect(ServerAddress server);
	void send(std::string_view message) const;
	std::string receive(std::size_t max_bytes = 1024) const;
	std::string request_reply(std::string_view message, std::size_t max_bytes = 1024) const;

private:
	SocketHandle socket_;
};
