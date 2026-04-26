#include "echoserver.h"

#include <netinet/in.h>
#include <sys/socket.h>

#include <iostream>
#include <stdexcept>

#include "serveraddress.h"
#include "socketerror.h"
#include "sockethandle.h"


namespace {


void send_all(int fd, const char* data, std::size_t len)
{
	std::size_t sent = 0;
	while (sent < len) {
		const ssize_t n = ::send(fd, data + sent, len - sent, 0);
		
		if (n < 0)
			throw SocketError("send failed");
		
		if (n == 0)
			throw std::runtime_error("send failed: connection closed");
		
		sent += static_cast<std::size_t>(n);
	}
}


}  // namespace


EchoServer::EchoServer(Port port)
	: port_(port)
{}


SocketHandle EchoServer::create_listen_socket() const
{
	SocketHandle server_socket(::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP));

	int opt = 1;
	if (::setsockopt(server_socket.get(), SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
		throw SocketError("setsockopt failed");

	return server_socket;
}


void EchoServer::bind_and_listen(const SocketHandle& server_socket) const
{
	const ServerAddress server_address = ServerAddress::any(port_);
	sockaddr_in server_addr = server_address.value();

	if (::bind(server_socket.get(), reinterpret_cast<sockaddr*>(&server_addr), sizeof(server_addr)) < 0)
		throw SocketError("bind failed");

	if (::listen(server_socket.get(), Backlog) < 0)
		throw SocketError("listen failed");
}


ClientConnection EchoServer::accept_client(const SocketHandle& server_socket) const
{
	return ClientConnection::accept_from(server_socket);
}


void EchoServer::serve_clients(const SocketHandle& server_socket) const
{
	while (true) {
		try {
			ClientConnection client = accept_client(server_socket);

			log_client_connected(client);
			handle_client(client);
		}
		catch (const std::exception& e) {
			std::cerr << "Client error: " << e.what() << '\n';
		}

		std::cout << "Client disconnected\n";
	}
}


void EchoServer::log_client_connected(const ClientConnection& client)
{
	std::cout
		<< "Client "
		<< client.address().ip_string()
		<< ":"
		<< client.address().port()
		<< " connected\n";
}


void EchoServer::run() const
{
	SocketHandle server_socket = create_listen_socket();
	bind_and_listen(server_socket);

	std::cout << "Echo server on port " << port_ << " (Ctrl+C to stop)\n";
	serve_clients(server_socket);
}


void EchoServer::handle_client(const ClientConnection& client)
{
	int client_fd = client.socket_fd();
	char buffer[BufferSize];
	
	while (true) {
		const ssize_t n = ::recv(client_fd, buffer, sizeof(buffer), 0);
		
		if (n < 0)
			throw SocketError("recv failed");
		
		if (n == 0)
			break;

		send_all(client_fd, buffer, static_cast<std::size_t>(n));
	}
}
