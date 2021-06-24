#pragma once
#include <SFML/Graphics.hpp>
#include "StateManager.h"

class Controller {
public:
	Controller();
	void run();

private:
	StateManager m_manager;
	sf::RenderWindow m_window;
};