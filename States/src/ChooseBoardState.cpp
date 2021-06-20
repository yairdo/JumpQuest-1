#include "ChooseBoardState.h"
#include "StateManager.h"
#include "Macros.h"
#include "BaseButton.h"
#include "Resources.h"
#include "ServerGameState.h"
#include "GameState.h"
#include "Server.h"

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
				static_cast<Server*>(m_networkObj.get())->startGame(startMessageCreator(0, 0));
				m_next = m_buttons[i]->ButtonState(m_manager, m_window, true, m_networkObj);
			}
			else
				/// change to i!!
				m_next = std::make_unique<GameState>(m_manager, m_window, true, m_networkObj/*, castle*/);
		}
	}
	if ((--m_buttons.end())->get()->checkCollision(loc))
		m_manager.lastState();
	;
}


