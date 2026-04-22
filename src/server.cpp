#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cerrno>
#include <cstring>
#include <iostream>
#include <string>
#include <system_error>


namespace {


[[noreturn]] void throw_errno(const std::string& message) {
	throw std::system_error(errno, std::generic_category(), message);
}


void send_all(int fd, const char* data, size_t len) {
	size_t sent = 0;

	while (sent < len) {
		const ssize_t n = ::send(fd, data + sent, len - sent, 0);

		if (n < 0)
			throw_errno("send failed");

		if (n == 0)
			throw std::runtime_error("send failed: connection closed");

		sent += static_cast<size_t>(n);
	}
}


class SocketHandle {
public:
	SocketHandle() 
		: fd_(::socket(AF_INET, SOCK_STREAM, 0)) 
	{
		if (fd_ < 0)
			throw_errno("socket failed");
	}

	explicit SocketHandle(int fd) 
		: fd_(fd) 
	{
		if (fd_ < 0)
			throw_errno("socket failed");
	}

	~SocketHandle() {
		if (fd_ >= 0)
			::close(fd_);
	}

	SocketHandle(const SocketHandle&) = delete;
	SocketHandle& operator=(const SocketHandle&) = delete;

	SocketHandle(SocketHandle&& other) noexcept : fd_(other.fd_) {
		other.fd_ = -1;
	}

	SocketHandle& operator=(SocketHandle&& other) noexcept {
		if (this != &other) {
			if (fd_ >= 0)
				::close(fd_);

			fd_ = other.fd_;
			other.fd_ = -1;
		}

		return *this;
	}

	int get() const { 
		return fd_; 
	}

private:
	int fd_;
};


class EchoServer {
public:
	explicit EchoServer(int port) : port_(port) {}

	void run() const {
		SocketHandle server_socket;

		int opt = 1;
		if (::setsockopt(server_socket.get(), SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
			throw_errno("setsockopt failed");
		}

		sockaddr_in server_addr{};
		server_addr.sin_family = AF_INET;
		server_addr.sin_addr.s_addr = INADDR_ANY;
		server_addr.sin_port = htons(port_);

		if (::bind(server_socket.get(), reinterpret_cast<sockaddr*>(&server_addr), sizeof(server_addr)) < 0) {
			throw_errno("bind failed");
		}

		if (::listen(server_socket.get(), Backlog) < 0) {
			throw_errno("listen failed");
		}

		std::cout << "Echo server on port " << port_ << " (Ctrl+C to stop)\n";

		for (;;) {
			try {
				sockaddr_in client_addr{};
				socklen_t client_len = sizeof(client_addr);
				SocketHandle client_socket(::accept(server_socket.get(), reinterpret_cast<sockaddr*>(&client_addr), &client_len));

				char ip[INET_ADDRSTRLEN] = {0};
				::inet_ntop(AF_INET, &client_addr.sin_addr, ip, sizeof(ip));
				std::cout << "Client " << ip << ":" << ntohs(client_addr.sin_port) << " connected\n";

				handle_client(client_socket.get());
			} 
			catch (const std::exception& e) {
				std::cerr << "Client error: " << e.what() << '\n';
			}

			std::cout << "Client disconnected\n";
		}
	}

private:
	static void handle_client(int client_fd) {
		char buffer[BufferSize];

		for (;;) {
			const ssize_t n = ::recv(client_fd, buffer, sizeof(buffer), 0);
			if (n < 0) {
				throw_errno("recv failed");
			}
			if (n == 0) {
				break;
			}
			send_all(client_fd, buffer, static_cast<size_t>(n));
		}
	}

	static constexpr size_t BufferSize = 1024;
	static constexpr int Backlog = 5;

	int port_;
};


}  // namespace


int main() {
	constexpr int Port = 8080;

	try {
		const EchoServer server(Port);
		server.run();
	} catch (const std::exception& e) {
		std::cerr << "Server error: " << e.what() << '\n';
		return 1;
	}

	return 0;
}
