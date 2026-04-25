#pragma once

#include <cstddef>

#include "port.h"

struct sockaddr_in;
class SocketHandle;


class EchoServer {
public:
	explicit EchoServer(Port port);

	void run() const;

private:
	SocketHandle create_listen_socket() const;
	void bind_and_listen(const SocketHandle& server_socket) const;
	SocketHandle accept_client(const SocketHandle& server_socket, sockaddr_in& client_addr) const;
	void serve_clients(const SocketHandle& server_socket) const;
	static void log_client_connected(const sockaddr_in& client_addr);
	static void handle_client(int client_fd);

	static constexpr std::size_t BufferSize = 1024;
	static constexpr int Backlog = 5;

	Port port_;
};
