#include <Controller.h>
#include <Resources.h>

int main() {
	Resources::getResourceRef();
	Controller().run();
	return EXIT_SUCCESS;
}