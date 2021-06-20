#pragma once
#include "MenuState.h"
#include "SFML/Graphics.hpp"

class StateManager;

class ChooseBoardState : public MenuState {
public:
	ChooseBoardState(StateManager& manager, sf::RenderWindow& window,
		bool replace, std::shared_ptr<NetworkObject>net=nullptr);
	virtual ~ChooseBoardState() = default;
	virtual void updateNextState(const sf::Vector2f& loc) override;

private:
	template <class StateT>
	void makeButtons();
};


template <class StateT>
void ChooseBoardState::makeButtons() {
	auto recWidth = m_window.getSize().x * 0.7f;
	auto side = recWidth / (3.f + 3 * 0.2f);
	auto spaceBut = (recWidth - side * 3.f) / (3 - 1);
	auto pos = sf::Vector2f{ (m_window.getSize().x - recWidth) / 2.f + 0.5f * side,
		3.5f * getTitleHeight() };
	addButton <StateT>(blank, pos, side, side);
	pos.x += spaceBut + side;
	addButton <StateT>(blank, pos, side, side);
	pos.x += spaceBut + side;
	addButton<StateT>(blank, pos, side, side);
	auto width = Resources::getResourceRef().getButLen(back) * PIX4LET * 1.3f;
	pos = { m_window.getSize().x / 2.f, m_window.getSize().y - m_window.getSize().y / 10.f };
	auto butHeight = m_window.getSize().y - pos.y;
	m_buttons.emplace_back(std::make_unique<BaseButton>(back, pos, width, butHeight));
}