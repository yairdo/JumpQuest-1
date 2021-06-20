#include "NetworkGameState.h"
#include "Player.h"
#include <Projectile.h>
#include <StateManager.h>
#include <MultiplayerMenuState.h>

NetworkGameState::NetworkGameState(StateManager& manager, sf::RenderWindow& window, bool replace, std::shared_ptr<NetworkObject> net):
	GameState(manager,window,replace,net/*,m_board->getMap()*/)
{
	m_networkObj->setBoard(m_board.get());
	for (int i = 0; i < MAX_SERVER_PLAYERS; ++i) {
		if (m_networkObj->getMember(i) && m_networkObj->getInfo().m_info.m_id != m_networkObj->getMember(i)->m_info.m_id) {
			m_clones[m_networkObj->getMember(i)->m_info.m_id] = struct ClonePlayer(m_networkObj->getMember(i)->m_info.m_id);
		}
	}
}

void NetworkGameState::draw(){
	GameState::draw();
	for (auto clone : m_clones)
		m_window.draw(clone.second.m_sprite);
}

void NetworkGameState::updateBoard()
{

	try {
		updateNetwork();
		for (int i = 0; i < MAX_SERVER_PLAYERS; ++i) {
			if (m_clones.find(i) != m_clones.end() && !m_networkObj->getMember(i)) {
				m_clones.erase(i);
			}
		}
	}
	catch (std::exception& e) {
		//if (e.what() == SERVER_CONNECTION_LOST)
		//sf::Vector2f viewSize(m_window.getSize().x, m_window.getSize().y);
		//sf::View view(sf::Vector2f(float(m_window.getSize().x), float(m_window.getSize().y)), 
		//	sf::Vector2f(m_window.getSize()));
		//view.setViewport({ 0.f,0.f,1,1 });
		setView(m_window.getDefaultView());
		//m_window.clear();
		m_next = m_manager.build<MultiplayerMenuState>(m_manager, m_window, true, nullptr);
		return;
	}
	GameState::updateBoard();
	sendInfo();
	updateClonesLoc();
}

void NetworkGameState::updateClonesLoc() {
	for (int i = 0; i < MAX_SERVER_PLAYERS; ++i) {
		if (m_networkObj->getMember(i) &&
			m_networkObj->getInfo().m_info.m_id != m_networkObj->getMember(i)->m_info.m_id) {
			auto info = m_networkObj->getMember(i)->m_info;
			//auto = struct of the info coming back from the server
			//update map about the players new parameters from the info struct above
			auto it = m_clones.find(m_networkObj->getMember(i)->m_info.m_id);
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