#pragma once
#include "MenuState.h"
#include <SFML/Graphics.hpp>

class StateManger;

class StandardMenuState : public MenuState {
public:
	virtual ~StandardMenuState()=default;

protected:
	sf::Vector2f m_middle;

};

