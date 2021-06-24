#include <Controller.h>
#include <Resources.h>
#include <iostream>

int main() {
	try {
		Controller().run(); //runs the game
	}
	catch (std::exception& exp) {
		std::cout<<exp.what();
	}
	return EXIT_SUCCESS;
}