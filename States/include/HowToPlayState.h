#pragma once
#include "MenuState.h"
#include <SFML/Graphics.hpp>
class StateManager;
class NetworkObject;

class HowToPlayState : public MenuState {
public:
	HowToPlayState(StateManager& manager, sf::RenderWindow& window, bool, std::shared_ptr<NetworkObject> net = nullptr);
	virtual void draw() override;
private:
	sf::Sprite m_howTo;
	sf::RectangleShape m_rect;
};
