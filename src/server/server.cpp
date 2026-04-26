#include <iostream>

#include "echoserver.h"
#include "port.h"


int main()
try {
	constexpr const char* version = "1.0.7";
	std::cout << "Starting Echo Server v" << version << '\n';

	const EchoServer server(Port{8080});
	
	server.run();
	
	return 0;
} 
catch (const std::exception& e) {
	std::cerr << "Server error: " << e.what() << '\n';
	return 1;
}
