#include "ClientGameState.h"
#include "Player.h"
#include "Client.h"
//-----------------------------------------------------------------------------
void ClientGameState::updateNetwork() {
	if (!m_started)
		((Client*)m_networkObj.get())->sendImReady();
	m_networkObj->handleRequests(300);
	if (m_networkObj->gameStarted())
		m_started = true;
}
