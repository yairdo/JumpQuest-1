#include "ServerGameState.h"
#include "Server.h"

void ServerGameState::updateNetwork(){
	m_networkObj->handleRequests(50);


	m_lastUpdate += m_deltaTime;
	///change to member and use reserve
	std::vector<MovingObjInfo> vec;
	if (m_networkObj && m_lastUpdate >= 0.03) {
		for (int i = 1; i < m_board->numOfMovingObjs(); ++i) {
			vec.push_back(m_board->getInfo(i));
		}
		((Server*)m_networkObj.get())->sendNewInfo(vec);
		m_lastUpdate = 0;
		m_clock.restart().asSeconds();
	}
}
