#include "echoserver.h"

#include <netinet/in.h>
#include <sys/socket.h>

#include <cerrno>
#include <iostream>
#include <stdexcept>
#include <string>
#include <system_error>

#include "serveraddress.h"
#include "sockethandle.h"


namespace {


[[noreturn]] void throw_errno(const std::string& message)
{
	throw std::system_error(errno, std::generic_category(), message);
}


void send_all(int fd, const char* data, std::size_t len)
{
	std::size_t sent = 0;
	while (sent < len) {
		const ssize_t n = ::send(fd, data + sent, len - sent, 0);
		
		if (n < 0)
			throw_errno("send failed");
		
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
		throw_errno("setsockopt failed");

	return server_socket;
}


void EchoServer::bind_and_listen(const SocketHandle& server_socket) const
{
	const ServerAddress server_address = ServerAddress::any(port_);
	sockaddr_in server_addr = server_address.value();

	if (::bind(server_socket.get(), reinterpret_cast<sockaddr*>(&server_addr), sizeof(server_addr)) < 0)
		throw_errno("bind failed");

	if (::listen(server_socket.get(), Backlog) < 0)
		throw_errno("listen failed");
}


SocketHandle EchoServer::accept_client(const SocketHandle& server_socket, ClientAddress& client_addr) const
{
	socklen_t client_len = client_addr.sockaddr_size();
	return SocketHandle(::accept(server_socket.get(), client_addr.sockaddr_ptr(), &client_len));
}


void EchoServer::serve_clients(const SocketHandle& server_socket) const
{
	for (;;) {
		try {
			ClientAddress client_addr;
			SocketHandle client_socket = accept_client(server_socket, client_addr);

			log_client_connected(client_addr);
			handle_client(client_socket.get());
		}
		catch (const std::exception& e) {
			std::cerr << "Client error: " << e.what() << '\n';
		}

		std::cout << "Client disconnected\n";
	}
}


void EchoServer::log_client_connected(const ClientAddress& client_addr)
{
	std::cout << "Client " << client_addr.ip_string() << ":" << client_addr.port() << " connected\n";
}


void EchoServer::run() const
{
	SocketHandle server_socket = create_listen_socket();
	bind_and_listen(server_socket);

	std::cout << "Echo server on port " << port_.value() << " (Ctrl+C to stop)\n";
	serve_clients(server_socket);
}


void EchoServer::handle_client(int client_fd)
{
	char buffer[BufferSize];
	while (true) {
		const ssize_t n = ::recv(client_fd, buffer, sizeof(buffer), 0);
		
		if (n < 0)
			throw_errno("recv failed");
		
		if (n == 0)
			break;

		send_all(client_fd, buffer, static_cast<std::size_t>(n));
	}
}
