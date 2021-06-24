#include "MainMenuState.h"
#include "StateManager.h"
#include "Macros.h"
#include "MultiplayerMenuState.h"
#include "ChooseBoardState.h"
#include "HowToPlayState.h"

//-----------------------------------------------------------------------------
/*
	c-tor
*/
MainMenuState::MainMenuState(StateManager& manager,sf::RenderWindow& window,
			bool replace,std::shared_ptr<NetworkObject> net ):
	StandardMenuState(manager,window,replace,net,title,menuBackground,BUT4+SPACES3,BUT4)
{
	m_window.setView(m_window.getDefaultView());
	makeBut<ChooseBoardState>(singlePlayer);
	makeBut<MultiplayerMenuState>(multiplayer);
	makeBut<HowToPlayState>(help);
	float width= Resources::getResourceRef().getButLen(ext) * m_pix4let;
	m_buttons.emplace_back(std::make_unique<BaseButton>(ext, m_pos, width, m_butHeight, true));
	Resources::getResourceRef().playMusic(menu);
};