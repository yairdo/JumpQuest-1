#include <LobbyState.h>

LobbyState::LobbyState(StateManager& manager, sf::RenderWindow& window, bool replace, std::shared_ptr<NetworkObject>& net) :
	MenuState(manager, window, replace, net, lobbyBackground), m_sighnedIn(false)
{
	//build prompt

	//add buttons
}
