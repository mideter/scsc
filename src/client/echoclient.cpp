#include "echoclient.h"

#include <arpa/inet.h>
#include <sys/socket.h>

#include <vector>

#include "socketerror.h"


EchoClient::EchoClient()
	: socket_(::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP))
{}


void EchoClient::connect(const ServerAddress& server_address)
{
	sockaddr_in server_addr = server_address.value();
	if (::connect(socket_.get(), reinterpret_cast<sockaddr*>(&server_addr), sizeof(server_addr)) < 0)
		throw SocketError("connect failed");
}


void EchoClient::send(std::string_view message) const
{
	std::size_t total_sent = 0;
	while (total_sent < message.size()) {
		const ssize_t sent = ::send(
			socket_.get(),
			message.data() + total_sent,
			message.size() - total_sent,
			0
		);

		if (sent < 0)
			throw SocketError("send failed");

		if (sent == 0)
			throw SocketError(EPIPE, "send failed");

		total_sent += static_cast<std::size_t>(sent);
	}
}


std::string EchoClient::receive(std::size_t max_bytes) const
{
	if (max_bytes == 0)
		return {};

	std::vector<char> buffer(max_bytes + 1, '\0');
	const ssize_t bytes_received = ::recv(socket_.get(), buffer.data(), max_bytes, 0);
	if (bytes_received < 0)
		throw SocketError("recv failed");

	return std::string(buffer.data(), static_cast<std::size_t>(bytes_received));
}


std::string EchoClient::request_reply(std::string_view message, std::size_t max_bytes) const
{
	send(message);
	return receive(max_bytes);
}
