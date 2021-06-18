#include "MainMenuState.h"
#include "StateManager.h"
#include "Macros.h"
#include "MultiplayerMenuState.h"
#include "GameState.h"
MainMenuState::MainMenuState(StateManager& manager,sf::RenderWindow& window,
			bool replace,std::shared_ptr<NetworkObject> net ):
	StandardMenuState(manager,window,replace,net,title,menuBackground)
{
	auto buttonSpace=m_window.getSize().y * 0.05;
	auto butHeight = (window.getSize().y - getTitlePosY()
		- buttonSpace*2 - buttonSpace * 5 ) / 4;
	float pix4let = m_window.getSize().x * 0.025;
	auto pos = sf::Vector2f(m_middle.x, getTitlePosY() + buttonSpace * 2.5);
	makeBut<GameState>(pos, singlePlayer, butHeight, pix4let, buttonSpace);
	makeBut<MultiplayerMenuState>(pos, multiplayer, butHeight, pix4let, buttonSpace);

	//addButton<howToPlayState>(help,pos,width,butHeight);
	pos.y += buttonSpace + butHeight;

	float width= Resources::getResourceRef().getButLen(ext) * pix4let;
	
	m_buttons.emplace_back(std::make_unique<BaseButton>(ext, pos, width, butHeight, true));
};