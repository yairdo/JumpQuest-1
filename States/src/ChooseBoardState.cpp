#include "ChooseBoardState.h"
#include "StateManager.h"
#include "Macros.h"
#include "BaseButton.h"
#include "Resources.h"
#include "ServerGameState.h"
#include "GameState.h"
#include "Server.h"
#include "MainMenuState.h"

ChooseBoardState::ChooseBoardState(StateManager& manager, sf::RenderWindow& window,
	bool replace, std::shared_ptr<NetworkObject>net) :
	MenuState(manager, window, replace, net, chooseBoard, lobbyBackground) {
	if (net)
		makeButtons<ServerGameState>();
	else
		makeButtons<GameState>();

}

void ChooseBoardState::updateNextState(const sf::Vector2f& loc) {
	for (int i=0; i<m_buttons.size()-1; ++i) {
		if (m_buttons[i]->checkCollision(loc)) {
			if (m_networkObj) {
				//change To i
				static_cast<Server*>(m_networkObj.get())->startGame(MapType(i));
				m_next = m_buttons[i]->ButtonState(m_manager, m_window, true, m_networkObj);
			}
			else
				/// change to i!!
				m_next = std::make_unique<GameState>(m_manager, m_window, true, m_networkObj, i);
		}
	}
	if ((--m_buttons.end())->get()->checkCollision(loc))
		//m_manager.lastState();
		m_next = std::make_unique<MainMenuState>(m_manager,m_window,true);
}


