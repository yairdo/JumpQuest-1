#include "MainMenuState.h"
#include "StateManager.h"
#include "Macros.h"
#include "MultiplayerMenuState.h"
#include "GameState.h"
#include "Resources.h"
#include "MenuState.h"
MainMenuState::MainMenuState(StateManager& manager,sf::RenderWindow& window,
			bool replace,std::shared_ptr<NetworkObject> net ):
	MenuState(manager,window,replace,net,title,menuBackground)
{
	auto buttonSpace=m_window.getSize().y * 0.05;
	auto startButPos = sf::Vector2f(m_middle.x, getTitlePosY() + buttonSpace *2.5);
	auto butHeight = (window.getSize().y - getTitlePosY()
		- buttonSpace*2 - buttonSpace * 5 ) / 4;
	float width;
	float pix4let = m_window.getSize().x * 0.025;
	auto pos = startButPos;
	width= Resources::getResourceRef().getButLen(singlePlayer) * pix4let;
	addButton<GameState>(singlePlayer,startButPos,width,butHeight);
	pos.y += buttonSpace + butHeight;
	width= Resources::getResourceRef().getButLen(multiplayer) * pix4let;
	addButton<MultiplayerMenuState>(multiplayer,pos,width,butHeight);
	pos.y += buttonSpace + butHeight;
	//addButton<howToPlayState>(help,pos,width,butHeight);
	width= Resources::getResourceRef().getButLen(ext) * pix4let;
	pos.y += buttonSpace + butHeight;
	m_buttons.emplace_back(std::make_unique<BaseButton>(ext, pos, width, butHeight, true));
};