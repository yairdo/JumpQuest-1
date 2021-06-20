#pragma once
#include "GameState.h"
#include <unordered_map>
#include "Macros.h"
class NetworkGameState:public GameState {
public:
	NetworkGameState(StateManager& manager, sf::RenderWindow& window, bool replace,
		std::shared_ptr<NetworkObject> net);
	virtual ~NetworkGameState()=default;
	virtual void draw() override;
	virtual void updateBoard() override;
	virtual void updateNetwork() = 0;
	

private:
	std::unordered_map<int, struct ClonePlayer> m_clones;
	void updateClonesLoc();
	void sendInfo();

};

struct ClonePlayer {
	ClonePlayer() = default;
	ClonePlayer(int id) :m_sprite(Resources::getResourceRef().getTexture(castle,player0+id)) {
		m_sprite.setTextureRect(sf::IntRect(0, 0, PLAYER_WIDTH, PLAYER_HEIGHT));
		m_sprite.setScale(50 / m_sprite.getGlobalBounds().width, 50 / m_sprite.getGlobalBounds().height);
		m_sprite.setOrigin(m_sprite.getTextureRect().width / 2.f, m_sprite.getTextureRect().height / 2.f);
		m_sprite.setPosition(50, 50);
	}
	float m_totalTime;
	sf::Sprite m_sprite;
	int m_row;
	int m_col;
	int m_direction;
};