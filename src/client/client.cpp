#include <iostream>
#include <string>

#include "echoclient.h"
#include "ipv4.h"
#include "port.h"
#include "serveraddress.h"


class SingaporeServer {
public:
	inline static const Port port{8080};
	inline static const IPv4 ip{"139.59.117.130"};
};


int main()
try {
	const ServerAddress server_address(SingaporeServer::ip, SingaporeServer::port);
	
	EchoClient client;
	client.connect(server_address);
	
	const std::string message = "Hello, I want to get it back!";
	const std::string reply = client.request_reply(message);
	
	std::cout << "Server replied: " << reply << '\n';

	return 0;
} 
catch (const std::exception& e) {
	std::cerr << e.what() << '\n';
	return 1;
}

