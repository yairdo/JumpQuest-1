#include "Controller.h"
#include "MainMenuState.h"
//-----------------------------------------------------------------------------
/*
* the main object- runs the game and the diffrent states
* the controller c-tor
*/
Controller::Controller() : m_window(sf::RenderWindow
(sf::VideoMode(sf::VideoMode::getDesktopMode()), "Jump Quest",
	sf::Style::Close | sf::Style::Titlebar |sf::Style::Fullscreen)), m_manager(m_window)
{
	m_window.setFramerateLimit(FRAME_RATE);
	sf::Texture text;
	text.loadFromFile("loading.png");
	sf::Sprite sprite(text);
	sprite.setOrigin(sprite.getGlobalBounds().width / 2.f, sprite.getGlobalBounds().height / 2.f);
	sprite.setPosition(m_window.getView().getCenter());
	m_window.draw(sprite);
	m_window.display();
	Resources::getResourceRef();
	m_manager.setStateManagerText();
}
//-----------------------------------------------------------------------------
//runs the same loop for the different states
void Controller::run() {
	m_manager.run(StateManager::build<MainMenuState>(m_manager, m_window, true, nullptr));
	while (m_manager.running()) {
		m_window.clear();
		m_manager.nextState();
		m_manager.draw();
		m_manager.update();
		m_window.display();
	}
	m_window.close();
}