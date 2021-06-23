#include <LobbyState.h>
#include <Server.h>
#include <iostream>
#include "Resources.h"
#include "MultiplayerMenuState.h"
#include "GameState.h"
#include "StateManager.h"
#include <cstring>
#include <MultiplayerMenuState.h>
#include "ServerGameState.h"
#include "ClientGameState.h"
#include "ChooseBoardState.h"
#include "MainMenuState.h"

//-----------------------------------------------------------------------------
LobbyState::LobbyState(StateManager& manager, sf::RenderWindow& window, bool replace,
	std::shared_ptr<NetworkObject>& net) :
	MenuState(manager, window, replace, net, lobbyTitle, lobbyBackground),
	m_connected(false), m_isServer(false), m_signedUp(false),
	m_listBackground({ window.getSize().x / THIRD, window.getSize().y / THIRD }),
	m_nameTextBox({ window.getSize().x / FORTH,window.getSize().y / FORTH })
{
	createBoxShape(m_listBackground, WHITE_TRANSP_CLR);
	createBoxShape(m_nameTextBox, BROWN_TRANSP_CLR);
	auto pos = sf::Vector2f{ m_nameTextBox.getPosition().x - (m_nameTextBox.getSize().x / 2) + 10,
		m_nameTextBox.getPosition().y - m_nameTextBox.getSize().y / 2 + 10 };
	m_text = createText(lobbyFont, 24, sf::Color::Black, "Enter your nickname:\n",pos);
	pos = { m_text.getPosition().x ,m_text.getPosition().y +
		m_text.getGlobalBounds().height + 10 };
	m_inputText = createText(lobbyFont, 24, sf::Color::Black, "", pos);

	float width = Resources::getResourceRef().getButLen(back) * PIX4LET * 1.3;
	pos = { width, m_window.getSize().y - m_window.getSize().y * TENTH_PREC };
	float butHeight = m_window.getSize().y - pos.y;
	addButton<MultiplayerMenuState>(back, pos, width, butHeight);
	//build prompt
	if ( typeid(*m_networkObj.get()).name() == typeid(Server).name()){
		m_connected = true;
		m_isServer = true;	
		width= Resources::getResourceRef().getButLen(start)* PIX4LET * 1.3f;
		pos.x = m_window.getSize().x - width;
		addButton<ChooseBoardState>(start, pos, width, butHeight);
		//try {
		//	m_connected = m_networkObj->launch();
		//}
		//catch (const std::exception& e) {
		//	m_next = m_manager.build<MainMenuState>(m_manager, m_window, true, nullptr);
		//	m_manager.setErrorMessage(e.what());
		////	return;
		//}
	}
	pos = m_nameTextBox.getPosition();
	if (m_isServer)
		m_waitingText = createText(lobbyFont, 50, sf::Color::Black, "Trying to open a room\n", pos, true);
	else
		m_waitingText = createText(lobbyFont, 50, sf::Color::Black, "Waiting for Host\n", pos, true);
	setNameListText();
}
//-----------------------------------------------------------------------------
void LobbyState::update(){
	if (!m_connected) {
		if (m_isServer) {
			m_next = StateManager::build<MultiplayerMenuState>
				(m_manager, m_window, true, nullptr);
			return;
		}
		//try {
			if (!m_networkObj->launch())
				m_networkObj->handleRequests(10);

			else
				m_connected = true;
			MenuState::update();
			return;
		//}
		//catch (const std::exception& e) {
		//	m_next = m_manager.build<MultiplayerMenuState>(m_manager, m_window, true, nullptr);
		//	m_manager.setErrorMessage(e.what());
		//	return;
		//}
	}
	if (!m_signedUp)
		signUp();
	if (m_connected && m_signedUp)
		MenuState::update();
	//try {
		if (m_networkObj->handleRequests()) {
			if (m_networkObj->getStarted())
				m_next = StateManager::build<ClientGameState>(m_manager, m_window, true, m_networkObj);
			else
				updateList();
		}
	//}
	/*catch (std::exception& e) {
		m_next = m_manager.build<MultiplayerMenuState>(m_manager, m_window, true, nullptr);
		m_manager.setErrorMessage(e.what());
		return;
	}*/
}
//-----------------------------------------------------------------------------
void LobbyState::signUp() {
	for (auto event = sf::Event{}; m_window.pollEvent(event);) {
		if (event.type == sf::Event::TextEntered) {
			if (event.text.unicode < 128) {
				if (event.text.unicode == 8) {
					if (m_inputStr.size() > 0)
						m_inputStr.pop_back();
				}
				else if (m_inputText.getString().getSize() < PLAYER_NAME_LEN) 
						m_inputStr += event.text.unicode;
				m_inputText.setString(m_inputStr);
				
			}
		}
		else if (event.type == sf::Event::KeyReleased) {
			if (event.key.code == sf::Keyboard::Enter) {
				m_networkObj->setName(m_inputStr.c_str());
				m_signedUp = true;
				updateList();
				break;
			}
		}
	}
}
//-----------------------------------------------------------------------------
void LobbyState::draw() {
	MenuState::draw();
	if (!m_connected) {
		m_window.draw(m_waitingText);
		return;
	}
	m_window.draw(m_listBackground);
	for (auto& name : m_nameList)
		m_window.draw(name);
	if(!m_signedUp){
		m_window.draw(m_nameTextBox);
		m_window.draw(m_text);
		m_window.draw(m_inputText);
	}
}
//-----------------------------------------------------------------------------
void LobbyState::updateList(){
	auto it = m_nameList.begin();
	for (int i = 0 ; i < MAX_SERVER_PLAYERS ; ++i) {
		if (m_networkObj->getMember(i)) {
			auto str = std::string(m_networkObj->getMember(i)->m_name);
			if (str != "") {
				it->setString(str);
				++it;
			}
		}
	}
	std::for_each(it, m_nameList.end(), [&](sf::Text&) { it->setString(""); });
}
//-----------------------------------------------------------------------------
void LobbyState::updateNextState(const sf::Vector2f& loc) {
	if (m_buttons[FIRST_BUT]->checkCollision(loc)) {
		m_next = m_buttons[FIRST_BUT]->ButtonState(m_manager, m_window, true, nullptr);
	}
	if (m_isServer && m_buttons[SEC_BUT]->checkCollision(loc)) {
		m_next = m_buttons[SEC_BUT]->ButtonState(m_manager, m_window, true, m_networkObj);
	}
}
//-----------------------------------------------------------------------------
void LobbyState::drawList(){
	for (auto& name : m_nameList) {
		m_window.draw(name);
	}
}
//-----------------------------------------------------------------------------
void LobbyState::setNameListText() {
	auto textHeight = (m_listBackground.getSize().y - 10 * MAX_SERVER_PLAYERS) / MAX_SERVER_PLAYERS;
	auto startPos = sf::Vector2f{
		m_listBackground.getPosition().x - m_listBackground.getSize().x / 2 + 10,
		m_listBackground.getPosition().y - m_listBackground.getSize().y / 2 + 10 };
	for (int i = 0; i < MAX_SERVER_PLAYERS; i++) {
		sf::Text text;
		text.setFont(Resources::getResourceRef().getFont(lobbyFont));
		text.setCharacterSize(24);
		text.setPosition(startPos);
		m_nameList.emplace_back(std::move(text));
		startPos.y += textHeight + 10;
	}
}
//-----------------------------------------------------------------------------
void LobbyState::createBoxShape(sf::RectangleShape& rec,const sf::Color& clr){
	rec.setFillColor(clr);
	rec.setOrigin({ rec.getSize().x / 2.f, rec.getSize().y / 2.f });
	rec.setPosition(m_window.getSize().x / 2.f,m_window.getSize().y / 2.f);
}
//-----------------------------------------------------------------------------
sf::Text LobbyState::createText(int font, int size,const sf::Color& clr,
		const std::string& str,const sf::Vector2f& pos, bool org){
	sf::Text txt;
	txt.setFont(Resources::getResourceRef().getFont(font));
	txt.setCharacterSize(size);
	txt.setString(str);
	txt.setFillColor(clr);
	if (org)
		txt.setOrigin({ txt.getGlobalBounds().width / 2.f, txt.getGlobalBounds().height / 2.f });
	txt.setPosition(pos);
	return std::move(txt);
}