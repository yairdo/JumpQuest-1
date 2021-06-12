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
	auto startButPos = sf::Vector2f(m_middle.x,getTitleHeight() + SPACE_BUTTONS * 2);
	auto butHeight = (window.getSize().y - getTitleHeight()
		- SPACE_BUTTONS * 2 -SPACE_BUTTONS* MAIN_MENU_BUTTONS) / MAIN_MENU_BUTTONS;
	float width;
	auto pos = startButPos;
	width= Resources::getResourceRef().getButLen(singlePlayer) * PIX4LET ;
	addButton<GameState>(singlePlayer,startButPos,width,butHeight);
	pos.y += SPACE_BUTTONS + butHeight;
	width= Resources::getResourceRef().getButLen(multiplayer) * PIX4LET ;
	addButton<MultiplayerMenuState>(multiplayer,pos,width,butHeight);
	pos.y += SPACE_BUTTONS + butHeight;
	//addButton<howToPlayState>(help,pos,width,butHeight);
	width= Resources::getResourceRef().getButLen(ext) * PIX4LET ;
	pos.y += SPACE_BUTTONS + butHeight;
	m_buttons.emplace_back(std::make_unique<BaseButton>(ext, pos, width, butHeight, true));
//	for (int i = 0; i < vec.size(); ++i) {
//		pos.y = m_startButPos.y + i * SPACE_BUTTONS + i * m_butHeight;
//		m_buttons.push_back(Button(vec[i], pos, m_butHeight));
//	}
};