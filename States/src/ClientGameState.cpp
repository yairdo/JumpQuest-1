#include "ClientGameState.h"
#include "Client.h"
#include "NetworkObject.h"

//-----------------------------------------------------------------------------
/*
	Function: update network:
	This function is updating the client.
*/
void ClientGameState::updateNetwork() {
	if (!m_started)
		((Client*)m_networkObj.get())->sendImReady();
	m_networkObj->handleRequests(CLIENT_STATE_MSG);
	if (m_networkObj->gameStarted())
		m_started = true;
}
