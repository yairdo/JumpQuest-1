#pragma once

#include "MenuState.h"

class LobbyState :public MenuState {
public:
	LobbyState(StateManager&, sf::RenderWindow&, bool, std::shared_ptr<NetworkObject>&);
	//virtual void update() override;
	virtual void draw() override;
	
private:
	sf::Sprite m_signInPrompt;
	sf::RectangleShape m_listBackground;
	sf::RectangleShape m_nameTextBox;
	sf::Text m_text;
	bool m_connected;
	bool m_isServer;

};