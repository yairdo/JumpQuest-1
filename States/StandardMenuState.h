#pragma once
#include "MenuState.h"
#include <SFML/Graphics.hpp>

class StateManger;

class StandardMenuState : public MenuState {
public:
	//StandardMenuState(Menuu& menu, const sf::Vector2f& windowSize,int menuBackground,int title);
	//StandardMenuState(int title);
	//StandardMenuState(StateManager& manager, sf::RenderWindow& window,
	//	bool replace, std::shared_ptr<NetworkObject> net, int title);
	virtual ~StandardMenuState()=default;
	//virtual void handleClick();
protected:
	sf::Vector2f m_middle;

};

