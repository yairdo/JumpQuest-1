#include <iostream>
#include <Server.h>
#include <Client.h>
#include <Lobby.h>
#include <Controller.h>
#include "box2d/box2d.h"

int main() {

	/*b2FixtureDef fix;
	std::cout << "density= "<<fix.density << std::endl;
	std::cout << "filter category bits= " << fix.filter.categoryBits << std::endl;
	std::cout << "filter group index = " << fix.filter.groupIndex << std::endl;
	std::cout << "filter category mask bits= " << "0xFFFF" << std::endl;
	std::cout << "friction = " << fix.friction << std::endl;
	std::cout << "sensor= " << ((fix.isSensor) ? "true\n" : "false\n");
	std::cout << "retitution= " << fix.restitution << std::endl;
	std::cout << "fix shape= " << typeid(fix.shape).name()<<std::endl ;*/
	Controller c;
	c.run();
	
	return EXIT_SUCCESS;
}