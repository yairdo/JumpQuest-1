#pragma once
#include "NetworkGameState.h"
class ServerGameState:public NetworkGameState {
public:
	using NetworkGameState::NetworkGameState;
	virtual ~ServerGameState()=default;
	virtual void updateNetwork() override;
private:
	float m_lastUpdate;
};