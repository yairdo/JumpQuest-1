#include "ClientGameState.h"
#include "Client.h"
#include "NetworkObject.h"

//-----------------------------------------------------------------------------
void ClientGameState::updateNetwork() {
	if (!m_started)
		((Client*)m_networkObj.get())->sendImReady();
	m_networkObj->handleRequests(300);
	if (m_networkObj->gameStarted())
		m_started = true;
}
