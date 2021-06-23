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
	MenuState(manager, window, replace, net, title, menuBackground)
{
	auto width = Resources::getResourceRef().getButLen(back) * PIX4LET ;
	auto tenthOfScreen = m_window.getSize().y / 10.f;
	sf::Vector2f pos{ m_window.getSize().x / 2.f, m_window.getSize().y -tenthOfScreen*0.7f  };
	auto butHeight = m_window.getSize().y - pos.y;
	addButton<MainMenuState>(back, pos, width, butHeight);
	//m_buttons.emplace_back(std::make_unique<BaseButton>(back, pos, width, butHeight));

	makeText();
	m_rect.setSize({ float(m_window.getSize().x-m_window.getSize().x/10.f),m_window.getSize().y - getTitleHeight() - (butHeight)*2.7f });
	
	m_rect.setFillColor(sf::Color{ 224, 224, 224, 150 });
	m_rect.setPosition({ m_window.getSize().x / 20.f,getTitleHeight()*1.5f });

}
//-----------------------------------------------------------------------------
void HowToPlayState::makeText() {
	m_text.setFont(Resources::getResourceRef().getFont(lobbyFont));
	std::string str;
	str = "Hello and welcome to Jump Quest\n";
	str += "Jump Quest is an obstacle course game.\nThe game has two modes : single - player mode and multiplayer mode.\n";
	str += "In single - player mode, you can train on the wanted map.\nIn multiplayer mode, the game becomes a racing game.\n";
	str += "The main goal is to reach as fast as you can to the finish line.\nThe first player to cross the finish line is the winner.\n";
	str += "At the course, you will see some treasure boxes.\nIf you collect one, you will receive a projectile that could help you to knock down other players.\n";
	str += "Additionally, there are checkpoints at some points on the map.\n";
	str += "\nGame Controls :\n";
	str += "Arrow keys or W, A, S, D keys for moving.\n";
	str += "Space - Bar for jumping.\n";
	str += "Left - click on a mouse for shooting a projectile to the clicked point after collecting a gift.\n";
	str += "Escape(while playing) to open the pause menu.\n";
	str += "\nEnjoy!\n";
	m_text.setString(str);
	m_text.setCharacterSize(28);
	//m_text.setScale(m_rect.getSize().x / m_text.getGlobalBounds().width, m_rect.getSize().y / m_text.getGlobalBounds().height);
	m_text.setFillColor(sf::Color::White);
	m_text.setOutlineColor(sf::Color::Black);
	m_text.setOutlineThickness(4);
	m_text.setOrigin(m_text.getGlobalBounds().width/2, m_text.getGlobalBounds().height / 2);
	m_text.setPosition(m_window.getView().getCenter().x, m_window.getView().getCenter().y+70);

}
//-----------------------------------------------------------------------------
void HowToPlayState::draw() {
	MenuState::draw();
	m_window.draw(m_rect);
	m_window.draw(m_text);
}
