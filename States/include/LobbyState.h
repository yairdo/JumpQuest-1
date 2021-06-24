#pragma once
#include <vector>
#include "MenuState.h"

class LobbyState :public MenuState {
public:
	//-----------c-tor------------

	LobbyState(StateManager&, sf::RenderWindow&, bool, std::shared_ptr<NetworkObject>&);

	//-----------d-tor------------

	virtual ~LobbyState() = default;


	virtual void update() override;
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
	void updateNextState(const sf::Vector2f& loc);
	void setNameListText();
	void createBoxShape(sf::RectangleShape&,const sf::Color& );
	void createTexts();
	sf::Text createText(int, int,const sf::Color&, const std::string&,
		const sf::Vector2f&, bool org=false);
	void signUp();
};