#include "ChooseBoardState.h"
#include "StateManager.h"
#include "Macros.h"
#include "BaseButton.h"
#include "Resources.h"
#include "ServerGameState.h"
#include "GameState.h"

ChooseBoardState::ChooseBoardState(StateManager& manager, sf::RenderWindow& window,
	bool replace, std::shared_ptr<NetworkObject>net) :
	MenuState(manager, window, replace, net, chooseBoard, lobbyBackground) {
	if (net)
		makeButtons<ServerGameState>();
	else
		makeButtons<GameState>();

}

void ChooseBoardState::updateNextState(const sf::Vector2f& loc) {
	//for (auto but = m_buttons.begin(); but != (m_buttons.end() - 1); ++but) {
	//	if (but->get()->checkCollision(loc)) {
	//		if (m_networkObj) {
	//			
	//		}
	//			//m_networkObj
	//	}
	//}
	//if ((--m_buttons.end())->get()->checkCollision(loc))
	//	m_manager.lastState();
	;
}


