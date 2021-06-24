#pragma once
#include <SFML/Graphics.hpp>
#include "StandardMenuState.h"

class StateManager;

class GameMenuState :public StandardMenuState {
public:
	//-----------c-tor------------
	GameMenuState(StateManager& m_manager, sf::RenderWindow& m_window, bool);
	
	//-----------d-tor------------
	virtual ~GameMenuState() = default;

	//----------Fuinction---------

	virtual void updateNextState(const sf::Vector2f& loc);
	virtual void update() override;
	virtual void draw() override;
	
private:
	//------Private Functions-----

	void updateButtonsPos();
};