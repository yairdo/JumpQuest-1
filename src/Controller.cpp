#include "Controller.h"
#include "MainMenuState.h"

Controller::Controller() : m_window(sf::RenderWindow
(sf::VideoMode(), "Jump Quest",
	sf::Style::Close | sf::Style::Titlebar |sf::Style::Fullscreen))
{
	m_window.setFramerateLimit(FRAME_RATE);
}
#include<GameState.h>
//#include <Server.h>
#include "ChooseBoardState.h"
#include "GameState.h"
void Controller::run() {
	//m_manager.run(StateManager::build<MainMenuState>(m_manager, m_window, true, nullptr));
	m_manager.run(StateManager::build<MainMenuState>(m_manager, m_window, true, nullptr));
	//m_manager.run(StateManager::build<GameState>(m_manager, m_window, true, nullptr));
	//m_manager.run(StateManager::build<ServerGameState>(m_manager, m_window, true, std::make_shared<Server>()));
	//m_manager.run(StateManager::build<ChooseBoardState>(m_manager, m_window, true));
	
	while (m_manager.running()) {
		m_window.clear();
		m_manager.nextState();
		m_manager.draw();
		m_manager.update();
		m_window.display();
	}
	m_window.close();
}