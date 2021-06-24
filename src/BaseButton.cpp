#include "BaseButton.h"
#include "Resources.h"
#include "Macros.h"

//-----------------------------------------------------------------------------
/*
	c-tor
*/
BaseButton::BaseButton(int index, const sf::Vector2f& pos, float width,
	float height, bool isExit): m_isExit(isExit){
	m_title.setTexture(Resources::getResourceRef().getTexture(index));
	m_title.setOrigin(m_title.getGlobalBounds().width / 2, m_title.getGlobalBounds().height / 2);
	m_title.setScale(width /m_title.getGlobalBounds().width,
		height / m_title.getGlobalBounds().height);
	m_title.setPosition(pos);
	m_title.setColor(sf::Color(255,255,255,200));
}
//-----------------------------------------------------------------------------
void BaseButton::draw(sf::RenderWindow& window) const{
	window.draw(m_title);
}
//-----------------------------------------------------------------------------
/*
	Function: set transp
	This function is setting the buttons half tranperence beside the one
	who the mouse stand on.
*/
void BaseButton::setTransp(const sf::Vector2f& loc) {
	if (m_title.getGlobalBounds().contains(loc))
		m_title.setColor(FULL_COLOR);
	else
		m_title.setColor(TRANSP_TEXTURE);
}
//-----------------------------------------------------------------------------
void BaseButton::setPos(float x) {
	m_title.setPosition({x,m_title.getPosition().y});
}