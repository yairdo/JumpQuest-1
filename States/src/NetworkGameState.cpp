#include "NetworkGameState.h"
#include "Player.h"
#include <Projectile.h>
#include <StateManager.h>
#include <MultiplayerMenuState.h>

//-----------------------------------------------------------------------------
NetworkGameState::NetworkGameState(StateManager& manager, sf::RenderWindow& window, bool replace,
	std::shared_ptr<NetworkObject> net) :
	GameState(manager, window, replace, net, net->getLvlInfo()),
	m_started(false)
{
	m_networkObj->setBoard(m_board.get());
	for (int i = 0; i < MAX_SERVER_PLAYERS; ++i) {
		if (m_networkObj->getMember(i) && m_networkObj->getInfo().m_info.m_id != m_networkObj->getMember(i)->m_info.m_id) {
			m_clones[m_networkObj->getMember(i)->m_info.m_id] = struct ClonePlayer(m_networkObj->getMember(i)->m_info.m_id,
				m_networkObj->getMember(i)->m_name);
		}
	}
}
//-----------------------------------------------------------------------------
void NetworkGameState::draw(){
	GameState::draw();
	for (auto clone : m_clones) {
		m_window.draw(clone.second.m_sprite);
		m_window.draw(clone.second.m_name);
	}
}

//-----------------------------------------------------------------------------
void NetworkGameState::updateBoard()
{
	try {
		updateNetwork();
		if (!m_started)
			return;
		for (int i = 0; i < MAX_SERVER_PLAYERS; ++i) {
			if (m_clones.find(i) != m_clones.end() && !m_networkObj->getMember(i)) {
				m_clones.erase(i);
			}
		}
	}
	catch (std::exception& e) {
		setView(m_window.getDefaultView());
		m_next = m_manager.build<MultiplayerMenuState>(m_manager, m_window, true, nullptr);
		return;
	}
	GameState::updateBoard();
	sendInfo();
	updateClonesLoc();
}
//-----------------------------------------------------------------------------
void NetworkGameState::updateClonesLoc() {
	for (int i = 0; i < MAX_SERVER_PLAYERS; ++i) {
		if (m_networkObj->getMember(i) &&
			m_networkObj->getInfo().m_info.m_id != m_networkObj->getMember(i)->m_info.m_id) {
			auto info = m_networkObj->getMember(i)->m_info;
			auto it = m_clones.find(m_networkObj->getMember(i)->m_info.m_id);
			if (it == m_clones.end())
				continue;
			it->second.m_sprite.setPosition(info.m_loc);
			it->second.m_name.setPosition(info.m_loc.x, info.m_loc.y- PLAYER_SIZE.y/1.5f);
			it->second.m_row = info.m_row;
			it->second.m_col = info.m_col;
			it->second.m_totalTime = info.m_totalTime;
			it->second.m_direction = info.m_direction;
			it->second.m_sprite.setTextureRect(Animation::getAnimRef().updateAnim(it->second.m_row, it->second.m_col,
				m_deltaTime, it->second.m_totalTime, player0, it->second.m_direction));
		}
	}
}
//-----------------------------------------------------------------------------
void NetworkGameState::sendInfo() {
	MemberInfo info = m_networkObj->getInfo().m_info;
	info.m_row = m_testPlayer->getAnimRow();
	info.m_col = m_testPlayer->getAnimCol();
	info.m_loc = m_testPlayer->getPos();
	info.m_direction = m_testPlayer->getDirection();
	m_networkObj->updateLoc(info);
}
//-----------------------------------------------------------------------------
void NetworkGameState::updateWin() {
	if (!m_isWin && m_testPlayer->getWin() && m_networkObj->getWinner() == MAX_SERVER_PLAYERS) {
		m_networkObj->notifyWinning();
	}
	else if (!m_isWin && m_networkObj->getWinner() != MAX_SERVER_PLAYERS) {
		std::string str;
		if (m_networkObj->getWinner() == m_networkObj->getInfo().m_info.m_id)
			str = "You";
		else
			str = (m_networkObj->getMember(m_networkObj->getWinner()))->m_name;
		str += " Won!";
		m_winnerText->setString(str);
		m_winnerText->setOrigin(m_winnerText->getGlobalBounds().width / 2.f, m_winnerText->getGlobalBounds().height / 2.f);
		m_isWin = true;
	}
}