#include "ClientGameState.h"
#include "Client.h"
#include "Player.h"
//-----------------------------------------------------------------------------
void ClientGameState::updateNetwork() {
	((Client*)m_networkObj.get())->sendImReady();
	m_networkObj->handleRequests(300);
	if (m_networkObj->gameStarted())
		m_started = true;
}
