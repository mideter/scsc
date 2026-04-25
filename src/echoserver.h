#pragma once

#include <cstddef>

#include "port.h"


class EchoServer {
public:
	explicit EchoServer(Port port);

	void run() const;

private:
	static void handle_client(int client_fd);

	static constexpr std::size_t BufferSize = 1024;
	static constexpr int Backlog = 5;

	Port port_;
};
