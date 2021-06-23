#include "StandardMenuState.h"
#include "StateManager.h"
#include <SFML/Graphics.hpp>
#include "NetworkObject.h"
StandardMenuState::StandardMenuState(StateManager& manager, sf::RenderWindow& window,
	bool replace, std::shared_ptr<NetworkObject> net, int title, int backGround,
	float mulSpaces, float divHeight):
	MenuState(manager,window,replace,net,title,backGround)
{
	m_buttonSpace = m_window.getSize().y * BUTTUN_SPACE_PREC;
	m_pos = sf::Vector2f(m_middle.x, getTitlePosY() + m_buttonSpace * 2.5);
	m_butHeight = (window.getSize().y - getTitlePosY() - m_buttonSpace * (mulSpaces)) / divHeight;
	m_pix4let = m_window.getSize().x * PREC4LET_STAND;
}
