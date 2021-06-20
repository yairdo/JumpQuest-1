#pragma once
#include <SFML/Graphics.hpp>
#include "StandardMenuState.h"

class StateManager;

class PauseState:public StandardMenuState {
public:
	PauseState(StateManager& m_manager, sf::RenderWindow& m_window, bool);
	virtual ~PauseState() = default;
	void updateNextState(const sf::Vector2f& loc);
	virtual void update() override;
	virtual void draw() override;
	
private:
	void updateButtonsPos();
};