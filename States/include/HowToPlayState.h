#pragma once
#include "MenuState.h"
#include <SFML/Graphics.hpp>
class StateManager;
class NetworkObject;

class HowToPlayState : public MenuState {
public:
	//-----------c-tor------------
	HowToPlayState(StateManager& manager, sf::RenderWindow& window,
		bool, std::shared_ptr<NetworkObject> net = nullptr);

	//-----------d-tor------------
	virtual ~HowToPlayState() = default;

	//----------Functions---------
	virtual void draw() override;
private:
	//-----------Members----------
	sf::Sprite m_howTo;
	sf::RectangleShape m_rect;
};
