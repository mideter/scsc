#pragma once

#include <cstddef>

#include "clientaddress.h"
#include "sockethandle.h"


class ClientConnection {
public:
	ClientConnection(SocketHandle socket, ClientAddress address);

	static ClientConnection accept_from(const SocketHandle& server_socket);

	const ClientAddress& address() const noexcept;
	
	bool recv_some(char* buffer, std::size_t len, std::size_t& received) const;
	void send_all(const char* data, std::size_t len) const;

private:
	SocketHandle socket_;
	ClientAddress address_;
};
