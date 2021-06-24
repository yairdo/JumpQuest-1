#pragma once
#include "NetworkGameState.h"

class Projectile;

class ServerGameState:public NetworkGameState {
public:
	//-----------c-tor------------
	ServerGameState(StateManager& manager, sf::RenderWindow& window,
		bool replace, std::shared_ptr<NetworkObject> net);

	//-----------d-tor------------
	virtual ~ServerGameState()=default;


protected:
	//----------Functions---------

	virtual void updateNetwork() override;
private:
	//-----------Members----------

	float m_lastUpdate;
};