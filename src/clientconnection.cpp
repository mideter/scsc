#include "clientconnection.h"

#include <sys/socket.h>

#include <utility>


ClientConnection::ClientConnection(SocketHandle socket, ClientAddress address)
	: socket_(std::move(socket))
	, address_(std::move(address))
{}


ClientConnection ClientConnection::accept_from(const SocketHandle& server_socket)
{
	ClientAddress address;
	socklen_t client_len = address.sockaddr_size();

	SocketHandle socket(::accept(server_socket.get(), address.sockaddr_ptr(), &client_len));
	return ClientConnection(std::move(socket), std::move(address));
}


const ClientAddress& ClientConnection::address() const noexcept
{
	return address_;
}


int ClientConnection::socket_fd() const noexcept
{
	return socket_.get();
}
