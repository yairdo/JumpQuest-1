#pragma once
#include "StandardMenuState.h"
#include <SFML/Graphics.hpp>

class StateManager;
class NetworkObject;

class MainMenuState:  public MenuState{
public:
	MainMenuState(StateManager& m_manager,sf::RenderWindow& m_window, bool,std::shared_ptr<NetworkObject> net = nullptr);
	virtual ~MainMenuState()=default;
private:
};