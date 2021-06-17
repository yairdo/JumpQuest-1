#include "ServerGameState.h"
#include "Server.h"
#include "Projectile.h"

ServerGameState::ServerGameState(StateManager& manager, sf::RenderWindow& window, bool replace, std::shared_ptr<NetworkObject> net):
	NetworkGameState(manager,window,replace,net), m_lastUpdate(0)
{
	//------projectile test-------
	m_testProjectile = new Projectile(getWorldRef(), { 50, 1000 }, { 10, 10 }, b2_dynamicBody);
	//---------------------
}

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

	//------test projectile
	int x = 80;
	if (!m_testProjectile->getShot())
		m_testProjectile->shot(m_testProjectile->getPos(), { (float)x, 1050 });
	std::cout << m_testProjectile->getPos().x << " " << m_testProjectile->getPos().y << std::endl;
	if(m_testProjectile->getPos().x >= x && m_testProjectile->getPos().x <= x+1)
		std::cout << "x reached: " <<  m_testProjectile->getPos().x << " " << m_testProjectile->getPos().y << std::endl;
	//if (m_testProjectile->getPos().y >= 240 && m_testProjectile->getPos().y <= 241)
	//	std::cout << "y reached: " << m_testProjectile->getPos().x << " " << m_testProjectile->getPos().y << std::endl;
	m_testProjectile->updatePhysics(m_deltaTime);
	m_testProjectile->move();
	//------

}

void ServerGameState::draw(){
	NetworkGameState::draw();
	m_testProjectile->draw(m_window);
}
