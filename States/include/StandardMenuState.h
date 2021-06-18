#pragma once
#include "MenuState.h"
#include <SFML/Graphics.hpp>

class StateManger;

class StandardMenuState : public MenuState {
public:
	using MenuState::MenuState;
	virtual ~StandardMenuState()=default;

protected:
	template <class T>
	void makeBut(sf::Vector2f& pos, int index, float height, float pix4let, double buttonSpace);

};

template <class T>
void StandardMenuState::makeBut(sf::Vector2f& pos, int index, float height,
	float pix4let, double buttonSpace) 
{
	float width = Resources::getResourceRef().getButLen(index) * pix4let;
	addButton<T>(index, pos, width, height);
	pos.y += buttonSpace + height;
}

