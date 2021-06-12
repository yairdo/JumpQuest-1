#include <LobbyState.h>
#include <Server.h>
#include <iostream>
#include "Resources.h"
#include "MultiplayerMenuState.h"
#include "GameState.h"


LobbyState::LobbyState(StateManager& manager, sf::RenderWindow& window, bool replace, std::shared_ptr<NetworkObject>& net) :
	MenuState(manager, window, replace, net,lobbyTitle, lobbyBackground), m_connected(false), m_isServer(false),
	m_listBackground({window.getSize().x/3.f, window.getSize().y/3.f})
{
	m_listBackground.setFillColor(sf::Color(255, 255, 255, 50));
	m_listBackground.setOrigin({ m_listBackground.getSize().x / 2.f, m_listBackground.getSize().y / 2.f });
	m_listBackground.setPosition(window.getSize().x/2, window.getSize().y/2);
	sf::Vector2f pos = {0, m_window.getSize().y-m_window.getSize().y/10.f};
	float butHeight = m_window.getSize().y - pos.y;
	float width = Resources::getResourceRef().getButLen(back);

	addButton<MultiplayerMenuState>(back, pos, width, butHeight);
	//build prompt
	if ( typeid(*m_networkObj.get()).name() == typeid(Server).name()){
		m_isServer = true;	
		std::cout << "is server\n";
		width= Resources::getResourceRef().getButLen(start);
		pos.x = m_window.getSize().x - width;
		addButton<GameState>(start, pos, width, butHeight);
	}
	//add buttons
}

//void LobbyState::update(){
//	//client
//	//waiting for host - sending whoisfree
//	//connected to server - sending membership
//	//enter nickname
//	//add name to list
//	//handle requests
//
//	//host
//	//enter server nickname
//	//launch server
//	//server launched
//	//add name to list
//	//handle requests
//
//	//wait for server to start the game
//}

void LobbyState::draw() {
	MenuState::draw();
	m_window.draw(m_listBackground);
}
