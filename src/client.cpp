#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include <cerrno>
#include <cstring>
#include <iostream>
#include <string>
#include <system_error>

#include "sockethandle.h"


namespace {


constexpr int kPort = 8080;
// constexpr const char* kServerIp = "127.0.0.1";
constexpr const char* kServerIp = "139.59.117.130";

void print_errno(const std::string& message) {
	std::cerr << message << ": " << std::strerror(errno) << '\n';
}


}  // namespace


int main() {
	try {
		SocketHandle sock(::socket(AF_INET, SOCK_STREAM, 0));

		sockaddr_in server_addr{};
		server_addr.sin_family = AF_INET;
		server_addr.sin_port = htons(kPort);

		if (::inet_pton(AF_INET, kServerIp, &server_addr.sin_addr) <= 0) {
			print_errno("inet_pton failed");
			return 1;
		}

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
	} catch (const std::system_error& e) {
		std::cerr << e.what() << '\n';
		return 1;
	}
}

