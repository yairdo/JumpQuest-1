#pragma once
#include "GameState.h"
#include <unordered_map>
class NetworkGameState:public GameState {
public:
	//using::GameState::GameState;
	NetworkGameState(StateManager& manager, sf::RenderWindow& window, bool replace, std::shared_ptr<NetworkObject> net);
	virtual ~NetworkGameState()=default;
	virtual void draw() override;
	virtual void updateBoard() override;
	virtual void updateNetwork() = 0;


private:
	std::unordered_map<int, struct ClonePlayer> m_clones;
	void updateClonesLoc();
	void sendInfo();
};