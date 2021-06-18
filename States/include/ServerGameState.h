#pragma once
#include "NetworkGameState.h"

class Projectile;

class ServerGameState:public NetworkGameState {
public:
	using NetworkGameState::NetworkGameState;
	ServerGameState(StateManager& manager, sf::RenderWindow& window, bool replace, std::shared_ptr<NetworkObject> net);
	virtual ~ServerGameState()=default;
	virtual void updateNetwork() override;
	virtual void draw()override;
private:
	float m_lastUpdate;
};