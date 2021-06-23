#include "ServerGameState.h"
#include "Server.h"
#include "Projectile.h"

//-----------------------------------------------------------------------------
ServerGameState::ServerGameState(StateManager& manager, sf::RenderWindow& window, bool replace, std::shared_ptr<NetworkObject> net):
	NetworkGameState(manager,window,replace,net), m_lastUpdate(0)
{
	//------projectile test-------
	//m_testProjectile = new Projectile(getWorldRef(), { 50, 150 }, { 10, 10 }, b2_dynamicBody);
	//---------------------
}
//-----------------------------------------------------------------------------
void ServerGameState::updateNetwork(){
	m_networkObj->handleRequests(50);
	if (m_networkObj->gameStarted())
		m_started = true;
	if (!m_started)
		return;
	m_lastUpdate += m_deltaTime;
	/*projTimer -= m_deltaTime;*/
	///change to member and use reserve
	std::vector<MovingObjInfo> vec;
	if (m_networkObj && m_lastUpdate >= 0.1) {
		for (int i = 1; i < m_board->numOfMovingObjs(); ++i) {
			vec.push_back(m_board->getInfo(i));
		}
		((Server*)m_networkObj.get())->sendNewInfo(vec);
		m_lastUpdate = 0;
		m_clock.restart().asSeconds();
	}
}
//-----------------------------------------------------------------------------
void ServerGameState::draw(){
	NetworkGameState::draw();
	/*m_testProjectile->draw(m_window);*/
}
