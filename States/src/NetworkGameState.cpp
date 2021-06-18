#include "NetworkGameState.h"
#include "Player.h"
#include <Projectile.h>
#include <StateManager.h>
#include <MultiplayerMenuState.h>

NetworkGameState::NetworkGameState(StateManager& manager, sf::RenderWindow& window, bool replace, std::shared_ptr<NetworkObject> net):
	GameState(manager,window,replace,net)
{
	m_testProjectile = new Projectile(getWorldRef(), { 50, 150 }, { 10, 10 }, b2_dynamicBody);
	m_networkObj->setBoard(m_board.get());
	for (int i = 0; i < MAX_SERVER_PLAYERS; ++i) {
		if (m_networkObj->getMembers(i) && m_networkObj->getInfo().m_info.m_id != m_networkObj->getMembers(i)->m_info.m_id) {
			m_clones[m_networkObj->getMembers(i)->m_info.m_id] = struct ClonePlayer();
		}
		else if (m_networkObj->getMembers(i) && m_clones.find(i) != m_clones.end()) {
			m_clones.erase(i);
		}
	}
}

void NetworkGameState::draw(){
	GameState::draw();
	for (auto clone : m_clones)
		m_window.draw(clone.second.m_sprite);
	m_testProjectile->draw(m_window);
}

void NetworkGameState::updateBoard()
{

	static float projTimer = 3;
	projTimer -= m_deltaTime;

	if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
		m_testProjectile->shot(m_testPlayer->getPos(), sf::Vector2f(sf::Mouse::getPosition()));
	}
	if (!m_testProjectile->getShot() && projTimer <= 0)
		m_testProjectile->shot(m_testProjectile->getPos(), { 100, 300 });
	std::cout << m_testProjectile->getPos().x << " " << m_testProjectile->getPos().y << std::endl;
	if (m_testProjectile->getPos().x > 400)
		std::cout << m_testProjectile->getPos().x << " " << m_testProjectile->getPos().y << std::endl;
	m_testProjectile->updatePhysics(m_deltaTime);
	m_testProjectile->move();
	try {
		updateNetwork();
	}
	catch (std::exception& e) {
		if (e.what() == SERVER_CONNECTION_LOST)
			m_next = m_manager.build<MultiplayerMenuState>(m_manager, m_window, true, nullptr);
		return;
	}
	GameState::updateBoard();
	sendInfo();
	updateClonesLoc();
}

void NetworkGameState::updateClonesLoc() {
	for (int i = 0; i < MAX_SERVER_PLAYERS; ++i) {
		if (m_networkObj->getMembers(i) &&
			m_networkObj->getInfo().m_info.m_id != m_networkObj->getMembers(i)->m_info.m_id) {
			auto info = m_networkObj->getMembers(i)->m_info;
			//auto = struct of the info coming back from the server
			//update map about the players new parameters from the info struct above
			auto it = m_clones.find(m_networkObj->getMembers(i)->m_info.m_id);
			if (it == m_clones.end())
				continue;
			it->second.m_sprite.setPosition(info.m_loc);
			it->second.m_row = info.m_row;
			it->second.m_col = info.m_col;
			it->second.m_direction = info.m_direction;
		}
	}
}

void NetworkGameState::sendInfo() {
	MemberInfo info = m_networkObj->getInfo().m_info;
	info.m_row = m_testPlayer->getAnimRow();
	info.m_col = m_testPlayer->getAnimCol();
	info.m_loc = m_testPlayer->getPos();
	info.m_direction = m_testPlayer->getDirection();
	m_networkObj->updateLoc(info);
}