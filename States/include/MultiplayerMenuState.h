#pragma once
#include "StandardMenuState.h"
#include <SFML/Graphics.hpp>

class StateManager;
class NetworkObject;

class MultiplayerMenuState:  public StandardMenuState {
public:
	//-----------c-tor------------
	MultiplayerMenuState(StateManager& manager,sf::RenderWindow& window,
		bool,std::shared_ptr<NetworkObject>  net = nullptr);
	//-----------d-tor------------

	virtual ~MultiplayerMenuState()=default;

protected:
	//----------Functions---------
	virtual void updateNextState(const sf::Vector2f& loc) override ;
private:
};