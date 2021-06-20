#pragma once
#include "NetworkGameState.h"
#include "stack"
class MovingObj;

class ClientGameState : public NetworkGameState {
public:
	using NetworkGameState::NetworkGameState;
	virtual ~ClientGameState() = default;
	virtual void updateNetwork() override;
private:
	//std::stack<MovingObj*> movingObjsToUpdateServer;
};