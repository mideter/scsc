#pragma once

#include "clientaddress.h"
#include "sockethandle.h"


class ClientConnection {
public:
	ClientConnection(SocketHandle socket, ClientAddress address);

	static ClientConnection accept_from(const SocketHandle& server_socket);

	const ClientAddress& address() const noexcept;
	int socket_fd() const noexcept;

private:
	SocketHandle socket_;
	ClientAddress address_;
};
