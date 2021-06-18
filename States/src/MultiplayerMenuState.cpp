#include "MultiplayerMenuState.h"
#include "Macros.h"
#include "MainMenuState.h"
#include "Server.h"
#include "Client.h"
#include "LobbyState.h"

MultiplayerMenuState::MultiplayerMenuState(StateManager& manager,sf::RenderWindow& window,
	bool replace,std::shared_ptr<NetworkObject>net):
	StandardMenuState(manager, window, replace, net, title, menuBackground)
{
	auto buttonSpace = m_window.getSize().y * 0.05;
	auto startButPos = sf::Vector2f(m_middle.x, getTitlePosY() + buttonSpace*2.5);
	auto butHeight = (window.getSize().y - getTitlePosY()- buttonSpace *3 - buttonSpace * 4 ) / 3;
	float width;
	float pix4let = m_window.getSize().x * 0.025;
	auto pos = startButPos;
	makeBut<LobbyState>(pos, host, butHeight, pix4let, buttonSpace);
	makeBut<LobbyState>(pos, client, butHeight, pix4let, buttonSpace);
	makeBut<MainMenuState>(pos, back, butHeight, pix4let, buttonSpace);
}

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
