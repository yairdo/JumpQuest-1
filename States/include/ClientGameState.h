#pragma once
#include "NetworkGameState.h"
class ClientGameState : public NetworkGameState {
public:
	using NetworkGameState::NetworkGameState;
	virtual ~ClientGameState() = default;
	virtual void updateNetwork() override;
private:
};