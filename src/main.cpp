#include <iostream>
#include <Server.h>
#include <Client.h>
#include <Lobby.h>
#include <Controller.h>
int state = 1;

int main() {
	//if (state == 0) {
	//	char c = 'b';
	//	while (c != 'c' && c != 'h') {
	//		std::cout << "c for client h for host: ";
	//		std::cin >> c;
	//	}
	//	if (c == 'c') {
	//		Client client;
	//		client.run();
	//	}
	//	else if (c == 'h') {
	//		Server server;
	//		server.run();
	//	}
	//}
	//else if (state == 1) {
		Controller c;
		c.run();
	//}
	
	return EXIT_SUCCESS;
}