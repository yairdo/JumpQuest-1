#pragma once

#include "MenuState.h"

class LobbyState :public MenuState {
public:
	LobbyState(StateManager&, sf::RenderWindow&, bool, std::shared_ptr<NetworkObject>&);

private:
	sf::Sprite m_signInPrompt;
	bool m_sighnedIn;
};