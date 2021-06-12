#pragma once
#include "MenuState.h"
#include <SFML/Graphics.hpp>

class StateManager;
class NetworkObject;

class MultiplayerMenuState:  public MenuState{
public:
	MultiplayerMenuState(StateManager& manager,sf::RenderWindow& window, bool,std::shared_ptr<NetworkObject>  net = nullptr);
	virtual ~MultiplayerMenuState()=default;
	virtual void updateNextState(const sf::Vector2f& loc);
private:
};