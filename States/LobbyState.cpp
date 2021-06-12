#include <LobbyState.h>
#include <Server.h>
#include <iostream>
#include "Resources.h"
#include "MultiplayerMenuState.h"
#include "GameState.h"
#include "StateManager.h"
#include <cstring>


LobbyState::LobbyState(StateManager& manager, sf::RenderWindow& window, bool replace, std::shared_ptr<NetworkObject>& net) :
	MenuState(manager, window, replace, net, lobbyTitle, lobbyBackground), m_connected(false), m_isServer(false),
	m_listBackground({ window.getSize().x / 3.f, window.getSize().y / 3.f }), m_signedUp(false),
	m_nameTextBox({ window.getSize().x / 4.f,window.getSize().y / 4.f }),
	m_nameList(4)
{
	m_listBackground.setFillColor(sf::Color(255, 255, 255, 120));
	m_listBackground.setOrigin({ m_listBackground.getSize().x / 2.f, m_listBackground.getSize().y / 2.f });
	m_listBackground.setPosition(window.getSize().x/2, window.getSize().y/2);
	
	m_nameTextBox.setFillColor(sf::Color(102,0,0,150));
	m_nameTextBox.setOrigin({ m_nameTextBox.getSize().x / 2.f, m_nameTextBox.getSize().y / 2.f });
	m_nameTextBox.setPosition(window.getSize().x / 2.f, window.getSize().y / 2.f);

	m_text.setFont(Resources::getResourceRef().getFont(lobbyFont));
	m_text.setCharacterSize(24);
	m_text.setFillColor(sf::Color::Black);
	//- (m_nameTextBox.getSize().x / 2)+10
	m_text.setPosition({ m_nameTextBox.getPosition().x - (m_nameTextBox.getSize().x / 2) + 10,
		m_nameTextBox.getPosition().y - m_nameTextBox.getSize().y / 2+10 });
	m_text.setString("Enter your nickname:\n");
	
	m_inputText.setFont(Resources::getResourceRef().getFont(lobbyFont));
	m_inputText.setCharacterSize(24);
	m_inputText.setFillColor(sf::Color::Black);
	m_inputText.setPosition({ m_text.getPosition().x ,
		m_text.getPosition().y + m_text.getGlobalBounds().height + 10 });
	//m_inputText

	float width = Resources::getResourceRef().getButLen(back) * PIX4LET * 1.3;
	sf::Vector2f pos = { width, m_window.getSize().y - m_window.getSize().y / 10.f };
	//sf::Vector2f pos = { 0,0 };
	float butHeight = m_window.getSize().y - pos.y;
	addButton<MultiplayerMenuState>(back, pos, width, butHeight);
	//build prompt
	if ( typeid(*m_networkObj.get()).name() == typeid(Server).name()){
		m_isServer = true;	
		std::cout << "is server\n";
		width= Resources::getResourceRef().getButLen(start)* PIX4LET * 1.3;
		pos.x = m_window.getSize().x - width;
		addButton<GameState>(start, pos, width, butHeight);
		//m_connected = static_cast<Server*>(m_networkObj.get())->launch();
	}
	setNameListText();
	m_networkObj->launch();
}

void LobbyState::update(){
	if (m_networkObj->getStarted()) {
		m_next = StateManager::build<GameState>(m_manager, m_window, true, m_networkObj);
		return;
	}
	if (!m_signedUp){
		for(auto event = sf::Event{}; m_window.pollEvent(event);){
			if (event.type == sf::Event::TextEntered) {
				if (event.text.unicode < 128){
					if (m_inputText.getString().getSize() < PLAYER_NAME_LEN) {
						m_inputStr += event.text.unicode;
						m_inputText.setString(m_inputStr);
					}
				}
			}
			else if (event.type == sf::Event::KeyReleased) {
				if(event.key.code == sf::Keyboard::Enter){
					m_networkObj->setName(m_inputStr.c_str());
					m_signedUp == true;
					break;
				}
			}
		}
	}
	if (m_networkObj->handleRequests())
		updateList();
}

void LobbyState::draw() {
	MenuState::draw();
	m_window.draw(m_listBackground);
	for (auto& name : m_nameList)
		m_window.draw(name);
	if(!m_signedUp){
		m_window.draw(m_nameTextBox);
		m_window.draw(m_text);
		m_window.draw(m_inputText);
	}
}

void LobbyState::updateList(){
	auto it = m_nameList.begin();
	for (int i = 0 ; i < MAX_SERVER_PLAYERS ; ++i) {
		if (m_networkObj->getMembers(i)) {
			auto str = std::string(m_networkObj->getMembers(i)->m_name);
			if (str != "") {
				it->setString(str);
				++it;
			}
		}
	}
	std::for_each(it, m_nameList.end(), [&](sf::Text&) { it->setString(""); });
}

void LobbyState::drawList(){
	for (auto& name : m_nameList) {
		m_window.draw(name);
	}
}
void LobbyState::setNameListText() {
	auto textHeight = (m_listBackground.getSize().y - 10 * MAX_LIST_NAMES_SIZE) / MAX_LIST_NAMES_SIZE;
	auto startPos = sf::Vector2f{
		m_listBackground.getPosition().x - m_listBackground.getSize().x / 2 + 10,
		m_listBackground.getPosition().y - m_listBackground.getSize().y / 2 + 10 };
	for (int i = 0; i < MAX_LIST_NAMES_SIZE; i++) {
		sf::Text text;
		text.setFont(Resources::getResourceRef().getFont(lobbyFont));
		text.setCharacterSize(24);
		text.setPosition(startPos);
		m_nameList.emplace_back(std::move(text));
		startPos.y += textHeight + 10;
	}
}