#pragma once
#include "MenuState.h"
#include "SFML/Graphics.hpp"

class StateManager;

class ChooseBoardState : public MenuState {
public:
	//-----------c-tor------------
	ChooseBoardState(StateManager& manager, sf::RenderWindow& window,
		bool replace, std::shared_ptr<NetworkObject>net=nullptr);

	//-----------d-tor------------
	virtual ~ChooseBoardState() = default;

	//----------Function----------
	virtual void updateNextState(const sf::Vector2f& loc) override;

private:

	//------Private Functions-----
	template <class StateT>
	void makeButtons();
};
//-----------------------------------------------------------------------------
/*
	Function: make buttons
	This function is ordering the buttons of the choose board state.
*/
template <class StateT>
void ChooseBoardState::makeButtons() {
	auto recWidth = m_window.getSize().x * PREC70;
	auto side = recWidth / (BUT3 + BUT3 * 0.2f);
	auto spaceBut = (recWidth - side * BUT3) / (BUT3 - 1);
	auto pos = sf::Vector2f{ (m_window.getSize().x - recWidth) / 2.f + 0.5f * side,
		2.f * getTitlePosY() };
	addButton <StateT>(castleScreenShot, pos, side, side);
	pos.x += spaceBut + side;
	addButton <StateT>(hellScreenShot, pos, side, side);
	pos.x += spaceBut + side;
	addButton<StateT>(skyScreenShot, pos, side, side);
	auto width = Resources::getResourceRef().getButLen(mainMenu) * PIX4LET * 1.3f;
	pos = { m_window.getSize().x / 2.f, m_window.getSize().y - m_window.getSize().y / 10.f };
	auto butHeight = m_window.getSize().y - pos.y;
	m_buttons.emplace_back(std::make_unique<BaseButton>(mainMenu, pos, width, butHeight));
}