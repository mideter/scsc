#include "clientconnection.h"

#include <sys/socket.h>

#include <utility>


ClientConnection::ClientConnection(SocketHandle socket, ClientAddress address)
	: socket_(std::move(socket))
	, address_(std::move(address))
{}


ClientConnection ClientConnection::accept_from(const SocketHandle& server_socket)
{
	ClientAddress client;

	socklen_t client_len = sizeof(client.address_);
	SocketHandle socket(::accept(server_socket.get(), 
								 reinterpret_cast<sockaddr*>(&client.address_), 
								 &client_len));

	return ClientConnection(std::move(socket), std::move(client));
}


const ClientAddress& ClientConnection::address() const noexcept
{
	return address_;
}


int ClientConnection::socket_fd() const noexcept
{
	return socket_.get();
}
