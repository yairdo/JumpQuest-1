#pragma once
#include "GameState.h"
#include <unordered_map>
#include "Macros.h"

class NetworkGameState:public GameState {
public:
	//-----------c-tor------------

	NetworkGameState(StateManager& manager, sf::RenderWindow& window, bool replace,
		std::shared_ptr<NetworkObject> net);

	//-----------d-tor------------
	virtual ~NetworkGameState()=default;

	//----------Functions---------
	virtual void draw() override;
	

private:
	//-----------Members----------

	std::unordered_map<int, struct ClonePlayer> m_clones;

	//----------Functions---------

	void updateClonesLoc();
	void sendInfo();

protected:

	//-----------Members----------
	bool m_started;

	//----------Functions---------

	virtual void updateNetwork() = 0;
	virtual void updateBoard() override;
	virtual void updateWin() override;
};
//-----------------------------------------------------------------------------
/*
	struct to move info about players between client and server
*/
struct ClonePlayer {
	ClonePlayer() = default;
	ClonePlayer(int id,const std::string& name) :
	m_sprite(Resources::getResourceRef().getTexture(castle,player0+id)),
		m_totalTime(0.f),m_row(0),m_col(0),m_direction(0)
	{
		m_sprite.setTextureRect(sf::IntRect(0, 0, PLAYER_WIDTH, PLAYER_HEIGHT));
		m_sprite.setScale(PLAYER_SIZE.x / m_sprite.getGlobalBounds().width,
			PLAYER_SIZE.y / m_sprite.getGlobalBounds().height);
		m_sprite.setOrigin(m_sprite.getTextureRect().width / 2.f,
			m_sprite.getTextureRect().height / 2.f);
		//maybe setpos
		m_name.setFont(Resources::getResourceRef().getFont(lobbyFont));
		m_name.setString(name);
		m_name.setOrigin(m_name.getGlobalBounds().width / 2, m_name.getGlobalBounds().height / 2);
		m_name.setScale(PLAYERS_NAMES_SCALE);
		
	}
	sf::Text m_name;
	float m_totalTime;
	sf::Sprite m_sprite;
	int m_row;
	int m_col;
	int m_direction;
};