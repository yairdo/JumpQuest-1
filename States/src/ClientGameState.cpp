#include "ClientGameState.h"
#include "Player.h"
//-----------------------------------------------------------------------------
void ClientGameState::updateNetwork() {
	//m_networkObj->IAmReady();
	m_networkObj->handleRequests(300);
	//m_testPlayer->
	//update somehow the network of changes to moving obj
}
