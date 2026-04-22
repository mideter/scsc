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

	std::cout << "Server started on port " << kPort << '\n';
	std::cout << "Waiting for one client...\n";

	sockaddr_in client_addr{};
	socklen_t client_len = sizeof(client_addr);
	const int client_fd = ::accept(server_fd, reinterpret_cast<sockaddr*>(&client_addr), &client_len);
	if (client_fd < 0) {
		print_errno("accept failed");
		::close(server_fd);
		return 1;
	}

	char ip[INET_ADDRSTRLEN] = {0};
	::inet_ntop(AF_INET, &client_addr.sin_addr, ip, sizeof(ip));
	std::cout << "Client connected from " << ip << ":" << ntohs(client_addr.sin_port) << '\n';

	char buffer[kBufferSize] = {0};
	const ssize_t bytes_received = ::recv(client_fd, buffer, sizeof(buffer) - 1, 0);
	if (bytes_received < 0) {
		print_errno("recv failed");
		::close(client_fd);
		::close(server_fd);
		return 1;
	}

	buffer[bytes_received] = '\0';
	std::cout << "Received: " << buffer << '\n';

	const std::string response = "Hello from server!";
	if (::send(client_fd, response.c_str(), response.size(), 0) < 0) {
		print_errno("send failed");
		::close(client_fd);
		::close(server_fd);
		return 1;
	}

	std::cout << "Response sent. Shutting down.\n";

	::close(client_fd);
	::close(server_fd);
	return 0;
}

