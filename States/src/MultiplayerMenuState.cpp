#include "MultiplayerMenuState.h"
#include "Macros.h"
#include "MainMenuState.h"
#include "Server.h"
#include "Client.h"
#include "LobbyState.h"

//-----------------------------------------------------------------------------
MultiplayerMenuState::MultiplayerMenuState(StateManager& manager, sf::RenderWindow& window,
	bool replace, std::shared_ptr<NetworkObject>net) :
	StandardMenuState(manager, window, replace, net, title, menuBackground,BUT3+4,BUT3)
{
	/*auto buttonSpace = m_window.getSize().y * BUTTUN_SPACE_PREC;
	auto pos = sf::Vector2f(m_middle.x, getTitlePosY() + buttonSpace*2.5);
	auto butHeight = (window.getSize().y - getTitlePosY()- buttonSpace *(BUT3+4) ) / BUT3;
	float pix4let = m_window.getSize().x * PREC4LET_STAND;*/
	/*makeBut<LobbyState>(pos, host, butHeight, pix4let, buttonSpace);
	makeBut<LobbyState>(pos, client, butHeight, pix4let, buttonSpace);
	makeBut<MainMenuState>(pos, back, butHeight, pix4let, buttonSpace);*/
	makeBut<LobbyState>(host);
	makeBut<LobbyState>(client);
	makeBut<MainMenuState>(back);
}
//-----------------------------------------------------------------------------
void MultiplayerMenuState::updateNextState(const sf::Vector2f& loc){
	if (m_buttons[FIRST_BUT]->checkCollision(loc)){
		m_networkObj = std::make_shared<Server>();
		m_next = m_buttons[FIRST_BUT]->ButtonState(m_manager, m_window, true,m_networkObj);
	}
	if (m_buttons[SEC_BUT]->checkCollision(loc)) {
		m_networkObj = std::make_shared<Client>();
		m_next = m_buttons[SEC_BUT]->ButtonState(m_manager, m_window, true, m_networkObj);
	}
	if (m_buttons[THIRD_BUT]->checkCollision(loc))
		m_next = m_buttons[THIRD_BUT]->ButtonState(m_manager, m_window, true, m_networkObj);

}
