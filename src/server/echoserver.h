#pragma once

#include <cstddef>

#include "clientconnection.h"
#include "port.h"


class SocketHandle;


class EchoServer {
public:
	explicit EchoServer(Port port);

	void run() const;

private:
	SocketHandle 	create_listen_socket() const;
	void 			bind_and_listen(const SocketHandle& server_socket) const;
	void 			serve_clients(const SocketHandle& server_socket) const;
	
	static void log_client_connected(const ClientConnection& client_connection);
	static void handle_client(const ClientConnection& client);

	static constexpr std::size_t BufferSize = 1024;
	static constexpr int Backlog = 5;

	Port port_;
};
