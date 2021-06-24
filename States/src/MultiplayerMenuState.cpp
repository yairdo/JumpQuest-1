#include "MultiplayerMenuState.h"
#include "Macros.h"
#include "MainMenuState.h"
#include "Server.h"
#include "Client.h"
#include "LobbyState.h"

//-----------------------------------------------------------------------------
/*
	c-tor
*/
MultiplayerMenuState::MultiplayerMenuState(StateManager& manager, sf::RenderWindow& window,
	bool replace, std::shared_ptr<NetworkObject>net) :
	StandardMenuState(manager, window, replace, net, title, menuBackground,BUT3+4,BUT3)
{
	makeBut<LobbyState>(host);
	makeBut<LobbyState>(client);
	makeBut<MainMenuState>(back);
}
//-----------------------------------------------------------------------------
/*
	Function: update next state
	This function is making the pressed button move to the next state 
	(up to the button pressed).
*/
void MultiplayerMenuState::updateNextState(const sf::Vector2f& loc){
	if (m_buttons[FIRST_BUT]->checkCollision(loc)){
		m_networkObj = std::make_shared<Server>();
		m_networkObj->launch();
		m_next = m_buttons[FIRST_BUT]->ButtonState(m_manager, m_window, true,m_networkObj);
	}
	if (m_buttons[SEC_BUT]->checkCollision(loc)) {
		m_networkObj = std::make_shared<Client>();
		m_next = m_buttons[SEC_BUT]->ButtonState(m_manager, m_window, true, m_networkObj);
	}
	if (m_buttons[THIRD_BUT]->checkCollision(loc))
		m_next = m_buttons[THIRD_BUT]->ButtonState(m_manager, m_window, true, m_networkObj);

}
