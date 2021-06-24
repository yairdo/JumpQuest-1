#pragma once
#include "NetworkGameState.h"

class MovingObj;

class ClientGameState : public NetworkGameState {
public:
	//-----------c-tor------------
	using NetworkGameState::NetworkGameState;

	//-----------d-tor------------
	virtual ~ClientGameState() = default;

protected:
	//----------Fuinction---------
	virtual void updateNetwork() override;
private:
};