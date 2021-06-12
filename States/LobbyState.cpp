#include <LobbyState.h>
#include <Server.h>
#include <iostream>
#include "Resources.h"
#include "MultiplayerMenuState.h"
#include "GameState.h"
#include "StateManager.h"


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
	m_nameTextBox.setPosition(window.getSize().x / 2, window.getSize().y / 2);

	m_text.setFont(Resources::getResourceRef().getFont(lobbyFont));
	m_text.setString("Enter your nickname:\n");
	m_text.setCharacterSize(24);
	m_text.setFillColor(sf::Color::Black);
	m_text.setPosition({ m_nameTextBox.getPosition().x + m_nameTextBox.getPosition().x / 2+10,
		m_nameTextBox.getPosition().y - m_nameTextBox.getPosition().y / 2+10 });
	
	m_inputText.setFont(Resources::getResourceRef().getFont(lobbyFont));
	m_text.setCharacterSize(24);
	m_text.setFillColor(sf::Color::Black);
	m_text.setPosition({ m_text.getPosition().x ,
		m_text.getPosition().y + m_text.getCharacterSize() + 10 });
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
	//m_networkObj->launch();
	//add button

	//sf::Event event;
	//sf::String playerInput;
	//sf::Text playerText;
	//playerText.setPosition(60, 300);
	//playerText.setColor(sf::Color::Red);
}

void LobbyState::update(){
	//pollEvent();
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
					//set name recieved char* and not string
					//m_networkObj->setName(m_inputText.getString());
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
	if(!m_signedUp){
		m_window.draw(m_nameTextBox);
		m_window.draw(m_text);
		m_window.draw(m_inputText);
	}
	//m_draw
	m_window.draw(m_listBackground);
}

void LobbyState::updateList(){
	
}

void LobbyState::drawList(){
	for (auto& name : m_nameList)
		//m_window.draw(m_otherName);
		m_window.draw(name);
}
void LobbyState::setNameListText() {
	auto textHeight = (m_listBackground.getSize().y - 10 * MAX_LIST_NAMES_SIZE) / MAX_LIST_NAMES_SIZE;
	auto startPos = sf::Vector2f{
		m_listBackground.getPosition().x - m_listBackground.getPosition().x / 2 + 10,
		m_listBackground.getPosition().y - m_listBackground.getPosition().y / 2 + 10 };
	for (int i = 0; i < MAX_LIST_NAMES_SIZE; i++) {
		sf::Text text;
		text.setFont(Resources::getResourceRef().getFont(lobbyFont));
		text.setCharacterSize(24);
		m_text.setPosition(startPos);
		m_nameList.emplace_back(std::move(text));
		startPos.y += textHeight + 10;
	}
}