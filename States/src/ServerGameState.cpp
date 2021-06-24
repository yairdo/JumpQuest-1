#include "ServerGameState.h"
#include "Server.h"
#include "Projectile.h"

//-----------------------------------------------------------------------------
ServerGameState::ServerGameState(StateManager& manager, sf::RenderWindow& window, bool replace, std::shared_ptr<NetworkObject> net):
	NetworkGameState(manager,window,replace,net), m_lastUpdate(0)
{}
//-----------------------------------------------------------------------------
/*
	Function: update network
	This function is updating the clients about the game messages and sending 
	sever messages.
*/
void ServerGameState::updateNetwork(){
	m_networkObj->handleRequests(SERVER_STATE_MSG);
	if (m_networkObj->gameStarted())
		m_started = true;
	if (!m_started)
		return;
	m_lastUpdate += m_deltaTime;
	std::vector<MovingObjInfo> vec;
	if (m_networkObj && m_lastUpdate >= UPDATE_TIMER) {
		for (int i = 1; i < int(m_board->numOfMovingObjs()); ++i) {
			vec.push_back(m_board->getInfo(i));
		}
		((Server*)m_networkObj.get())->sendNewInfo(vec);
		m_lastUpdate = 0;
		m_clock.restart().asSeconds();
	}
}

