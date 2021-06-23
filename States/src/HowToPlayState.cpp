#include "HowToPlayState.h"
#include "StateManager.h"
#include "NetworkObject.h"
#include "Macros.h"
#include <memory>
#include "MainMenuState.h"
#include "BaseButton.h"

//-----------------------------------------------------------------------------
HowToPlayState::HowToPlayState(StateManager& manager, sf::RenderWindow& window, bool replace,
	std::shared_ptr<NetworkObject> net ):
	MenuState(manager, window, replace, net, title, menuBackground), 
	m_howTo(Resources::getResourceRef().getTexture(howTo))
{
	auto width = Resources::getResourceRef().getButLen(back) * PIX4LET ;
	auto tenthOfScreen = m_window.getSize().y * TENTH_PREC;
	sf::Vector2f pos{ m_window.getSize().x / 2.f, m_window.getSize().y -tenthOfScreen* PREC70 };
	auto butHeight = m_window.getSize().y - pos.y;
	addButton<MainMenuState>(back, pos, width, butHeight);

	m_rect.setSize({ float(m_window.getSize().x-m_window.getSize().x* TENTH_PREC),
		m_window.getSize().y - getTitleHeight() - (butHeight)*2.7f });
	
	m_howTo.setScale(m_rect.getGlobalBounds().width/m_howTo.getGlobalBounds().width, 
					 m_rect.getGlobalBounds().height / m_howTo.getGlobalBounds().height);
	m_howTo.setPosition({ m_window.getSize().x / 20.f,getTitleHeight() * 1.5f });

	m_rect.setFillColor(BLACK_TRANSP_CLR);
	m_rect.setPosition({ m_window.getSize().x / 20.f,getTitleHeight()*1.5f });

}

//-----------------------------------------------------------------------------
void HowToPlayState::draw() {
	MenuState::draw();
	m_window.draw(m_rect);
	
	m_window.draw(m_howTo);
}
