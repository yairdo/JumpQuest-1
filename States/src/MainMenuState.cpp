#include "MainMenuState.h"
#include "StateManager.h"
#include "Macros.h"
#include "MultiplayerMenuState.h"
#include "ChooseBoardState.h"
#include "HowToPlayState.h"

//-----------------------------------------------------------------------------
MainMenuState::MainMenuState(StateManager& manager,sf::RenderWindow& window,
			bool replace,std::shared_ptr<NetworkObject> net ):
	StandardMenuState(manager,window,replace,net,title,menuBackground,BUT4+SPACES3,BUT4)
{
	m_window.setView(m_window.getDefaultView());
	/*auto buttonSpace=m_window.getSize().y * BUTTUN_SPACE_PREC;
	auto butHeight = (window.getSize().y - getTitlePosY()
		- buttonSpace*(BUT4+SPACES3) ) / BUT4;
	float pix4let = m_window.getSize().x * PREC4LET_STAND;
	auto pos = sf::Vector2f(m_middle.x, getTitlePosY() + buttonSpace * 2.5f);
	makeBut<ChooseBoardState>(pos, singlePlayer, butHeight, pix4let, buttonSpace);
	makeBut<MultiplayerMenuState>(pos, multiplayer, butHeight, pix4let, buttonSpace);
	makeBut<HowToPlayState>(pos, help, butHeight, pix4let, buttonSpace);*/
	makeBut<ChooseBoardState>(singlePlayer);
	makeBut<MultiplayerMenuState>(multiplayer);
	makeBut<HowToPlayState>(help);
	float width= Resources::getResourceRef().getButLen(ext) * m_pix4let;
	m_buttons.emplace_back(std::make_unique<BaseButton>(ext, m_pos, width, m_butHeight, true));
	Resources::getResourceRef().playMusic(menu);
};