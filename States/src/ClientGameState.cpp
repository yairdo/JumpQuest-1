#include "ClientGameState.h"
#include "Player.h"
//-----------------------------------------------------------------------------
void ClientGameState::updateNetwork() {
	//m_networkObj->IAmReady();
	m_networkObj->handleRequests(300);
	if (m_networkObj->gameStarted())
		m_started = true;
}
