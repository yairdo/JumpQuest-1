#pragma once
#include "MenuState.h"
#include <SFML/Graphics.hpp>

class StateManger;

class StandardMenuState : public MenuState {
public:

	//-----------c-tor------------
	using MenuState::MenuState;
	StandardMenuState(StateManager& manager, sf::RenderWindow& window,
		bool replace, std::shared_ptr<NetworkObject> net, int title, int backGround,
		float mulSpaces, float divHeight);

	//-----------d-tor------------
	virtual ~StandardMenuState()=default;

protected:
	//----------Functions---------

	template <class T>
	void makeBut(sf::Vector2f& pos, int index, float height, float pix4let, double buttonSpace);

	template <class T>
	void makeBut(int index);

	//-----------Members----------

	float m_buttonSpace;
	sf::Vector2f m_pos;
	float m_butHeight;
	float m_pix4let;
};
//-----------------------------------------------------------------------------
/*
	Function: make button
	This function is creating standard menu (the buttons centered and going 
	up-down).
*/
template <class T>
void StandardMenuState::makeBut(int index)
{
	float width = Resources::getResourceRef().getButLen(index) * m_pix4let;
	addButton<T>(index, m_pos, width, m_butHeight);
	m_pos.y += float(m_buttonSpace + m_butHeight);
}

