#include "echoserver.h"

#include <netinet/in.h>
#include <sys/socket.h>

#include <iostream>

#include "serveraddress.h"
#include "socketerror.h"
#include "sockethandle.h"


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
	ServerAddress server = ServerAddress::any(port_);

	if (::bind(server_socket.get(), reinterpret_cast<sockaddr*>(&server.address_), sizeof(server.address_)) < 0)
		throw SocketError("bind failed");

	if (::listen(server_socket.get(), Backlog) < 0)
		throw SocketError("listen failed");
}


void EchoServer::serve_clients(const SocketHandle& server_socket) const
{
	while (true) {
		try {
			ClientConnection client = ClientConnection::accept_from(server_socket);

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
	std::cout << "Client " << client.address() << " connected" << std::endl;
}


void EchoServer::run() const
{
	SocketHandle server_socket = create_listen_socket();
	bind_and_listen(server_socket);

	std::cout << "Echo server on port " << port_ << " (Ctrl+C to stop)" << std::endl;
	serve_clients(server_socket);
}


void EchoServer::handle_client(const ClientConnection& client)
{
	char buffer[BufferSize];
	std::size_t received = 0;
	
	while (client.recv_some(buffer, sizeof(buffer), received))
		client.send_all(buffer, received);
}
