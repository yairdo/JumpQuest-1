#include "ClientGameState.h"

void ClientGameState::updateNetwork(){
	m_networkObj->handleRequests(300);
}
