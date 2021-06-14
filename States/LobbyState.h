#pragma once
#include <vector>
#include "MenuState.h"

class LobbyState :public MenuState {
public:
	LobbyState(StateManager&, sf::RenderWindow&, bool, std::shared_ptr<NetworkObject>&);
	virtual void update() override;
	void signUp();
	virtual void draw() override;
	
private:
	sf::Sprite m_signInPrompt;
	sf::RectangleShape m_listBackground;
	sf::RectangleShape m_nameTextBox;
	sf::Text m_text;
	sf::Text m_inputText;
	sf::Text m_waitingText;
	std::string	m_inputStr;
	bool m_connected;
	bool m_signedUp;
	bool m_isServer;
	std::vector<sf::Text> m_nameList;
	void drawList();
	void updateList();
	void setNameListText();
};