#pragma once
#include <SFML/Graphics.hpp>
#include "StandardMenuState.h"

class StateManager;

class GameMenuState :public StandardMenuState {
public:
	GameMenuState(StateManager& m_manager, sf::RenderWindow& m_window, bool);
	virtual ~GameMenuState() = default;
	void updateNextState(const sf::Vector2f& loc);
	virtual void update() override;
	virtual void draw() override;
	
private:
	void updateButtonsPos();
};