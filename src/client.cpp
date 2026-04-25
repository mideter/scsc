#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include <cerrno>
#include <cstring>
#include <iostream>
#include <string>

#include "ipv4address.h"
#include "port.h"
#include "serveraddress.h"
#include "sockethandle.h"


class SingaporeServer {
public:
	inline static const Port Port{8080};
	inline static const IPv4Address Ip{"139.59.117.130"};
};



void print_errno(const std::string& message) {
	std::cerr << message << ": " << std::strerror(errno) << '\n';
}


int main()
try {
	SocketHandle sock(::socket(AF_INET, SOCK_STREAM, 0));
	const ServerAddress server_address(SingaporeServer::Ip, SingaporeServer::Port);
	sockaddr_in server_addr = server_address.value();

	if (::connect(sock.get(), reinterpret_cast<sockaddr*>(&server_addr), sizeof(server_addr)) < 0) {
		print_errno("connect failed");
		return 1;
	}

	const std::string message = "Hello, I want to get it back!";
	if (::send(sock.get(), message.c_str(), message.size(), 0) < 0) {
		print_errno("send failed");
		return 1;
	}

	char buffer[1024] = {0};
	const ssize_t bytes_received = ::recv(sock.get(), buffer, sizeof(buffer) - 1, 0);
	if (bytes_received < 0) {
		print_errno("recv failed");
		return 1;
	}

	buffer[bytes_received] = '\0';
	std::cout << "Server replied: " << buffer << '\n';

	return 0;
} 
catch (const std::exception& e) {
	std::cerr << e.what() << '\n';
	return 1;
}

