#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cerrno>
#include <cstring>
#include <iostream>
#include <string>


namespace {


constexpr int kPort = 8080;
constexpr int kBacklog = 5;
constexpr size_t kBufferSize = 1024;


void print_errno(const std::string& message) {
	std::cerr << message << ": " << std::strerror(errno) << '\n';
}


bool send_all(int fd, const char* data, size_t len) {
	size_t sent = 0;
	while (sent < len) {
		const ssize_t n = ::send(fd, data + sent, len - sent, 0);
		if (n < 0) {
			print_errno("send failed");
			return false;
		}
		if (n == 0) {
			return false;
		}
		sent += static_cast<size_t>(n);
	}
	return true;
}


void handle_client(int client_fd) {
	char buffer[kBufferSize];

	for (;;) {
		const ssize_t n = ::recv(client_fd, buffer, sizeof(buffer), 0);
		if (n < 0) {
			print_errno("recv failed");
			break;
		}
		if (n == 0) {
			break;
		}
		if (!send_all(client_fd, buffer, static_cast<size_t>(n))) {
			break;
		}
	}
}


}  // namespace


int main() {
	const int server_fd = ::socket(AF_INET, SOCK_STREAM, 0);
	if (server_fd < 0) {
		print_errno("socket failed");
		return 1;
	}

	int opt = 1;
	if (::setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
		print_errno("setsockopt failed");
		::close(server_fd);
		return 1;
	}

	sockaddr_in server_addr{};
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port = htons(kPort);

	if (::bind(server_fd, reinterpret_cast<sockaddr*>(&server_addr), sizeof(server_addr)) < 0) {
		print_errno("bind failed");
		::close(server_fd);
		return 1;
	}

	if (::listen(server_fd, kBacklog) < 0) {
		print_errno("listen failed");
		::close(server_fd);
		return 1;
	}

	std::cout << "Echo server on port " << kPort << " (Ctrl+C to stop)\n";

	for (;;) {
		sockaddr_in client_addr{};
		socklen_t client_len = sizeof(client_addr);
		const int client_fd = ::accept(server_fd, reinterpret_cast<sockaddr*>(&client_addr), &client_len);
		if (client_fd < 0) {
			print_errno("accept failed");
			continue;
		}

		char ip[INET_ADDRSTRLEN] = {0};
		::inet_ntop(AF_INET, &client_addr.sin_addr, ip, sizeof(ip));
		std::cout << "Client " << ip << ":" << ntohs(client_addr.sin_port) << " connected\n";

		handle_client(client_fd);

		::close(client_fd);
		std::cout << "Client disconnected\n";
	}
}
