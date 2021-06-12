#include "BaseButton.h"
#include "Resources.h"
#include "Macros.h"
BaseButton::BaseButton(int index, const sf::Vector2f& pos, float width, float height, bool isExit): m_isExit(isExit){
	m_title.setTexture(Resources::getResourceRef().getTexture(index));
	m_title.setOrigin(m_title.getGlobalBounds().width / 2, m_title.getGlobalBounds().height / 2);
	m_title.setScale(width /m_title.getGlobalBounds().width, height / m_title.getGlobalBounds().height);
	m_title.setPosition(pos);
	//m_title.setColor(sf::Color(255,255,255,200));
}
//BaseButton::BaseButton(int index, const sf::Vector2f& pos, float height):
//	m_enumIndex(index){
//	float width = Resources::getResourceRef().getButLen(index)* PIX4LET;
//	m_title.setTexture(Resources::getResourceRef().getTexture(index));
//	m_title.setOrigin(m_title.getGlobalBounds().width / 2, m_title.getGlobalBounds().height / 2);
//	m_title.setScale(width / m_title.getGlobalBounds().width, height / m_title.getGlobalBounds().height);
//	m_title.setPosition(pos);
//
//}
void BaseButton::draw(sf::RenderWindow& window) const{
	window.draw(m_title);
}
void BaseButton::setTransp(const sf::Vector2f& loc) {
	if (m_title.getGlobalBounds().contains(loc))
		m_title.setColor(sf::Color(255, 255, 255, 255));
	else
		m_title.setColor(sf::Color(255, 255, 255, 200));
}