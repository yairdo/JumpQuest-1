#include "MultiplayerMenuState.h"
#include "Macros.h"
#include "GameState.h"
#include "MainMenuState.h"
#include "Server.h"
#include "Client.h"
#include "Resources.h"
#include "LobbyState.h"

MultiplayerMenuState::MultiplayerMenuState(StateManager& manager,sf::RenderWindow& window, bool replace,std::shared_ptr<NetworkObject>net):
	MenuState(manager, window, replace, nullptr, title, menuBackground)
{
	auto startButPos = sf::Vector2f(m_middle.x, getTitleHeight() + SPACE_BUTTONS * 2);
	auto butHeight = (window.getSize().y - getTitleHeight()
		- SPACE_BUTTONS * 2 -SPACE_BUTTONS* MAIN_MENU_BUTTONS) / MAIN_MENU_BUTTONS;
	float width;
	auto pos = startButPos;
	//host
	width= Resources::getResourceRef().getButLen(host) * PIX4LET ;
	addButton<LobbyState>(host,startButPos,width,butHeight);
	pos.y += SPACE_BUTTONS + butHeight;
	//client
	width= Resources::getResourceRef().getButLen(client) * PIX4LET ;
	addButton<LobbyState>(client,pos,width,butHeight);
	//back
	width= Resources::getResourceRef().getButLen(back) * PIX4LET ;
	pos.y += SPACE_BUTTONS + butHeight;
	addButton<MainMenuState>(back,pos,width,butHeight);
}
////////TEMP!!!!!
void MultiplayerMenuState::updateNextState(const sf::Vector2f& loc){
	if (m_buttons[0]->checkCollision(loc)){
		m_networkObj = std::make_shared<Server>();
		m_next = m_buttons[0]->ButtonState(m_manager, m_window, true,m_networkObj);
		}
	if (m_buttons[1]->checkCollision(loc)) {
		m_networkObj = std::make_shared<Client>();
		m_next = m_buttons[1]->ButtonState(m_manager, m_window, true, m_networkObj);
	}
	if (m_buttons[2]->checkCollision(loc))
		m_next = m_buttons[2]->ButtonState(m_manager, m_window, true, m_networkObj);

}
