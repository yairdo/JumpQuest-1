#include "MainMenuState.h"
#include "StateManager.h"
#include "Macros.h"
#include "MultiplayerMenuState.h"
#include "ChooseBoardState.h"
#include "HowToPlayState.h"

//-----------------------------------------------------------------------------
MainMenuState::MainMenuState(StateManager& manager,sf::RenderWindow& window,
			bool replace,std::shared_ptr<NetworkObject> net ):
	StandardMenuState(manager,window,replace,net,title,menuBackground)
{
	m_window.setView(m_window.getDefaultView());
	auto buttonSpace=m_window.getSize().y * 0.05f;
	auto butHeight = (window.getSize().y - getTitlePosY()
		- buttonSpace*2 - buttonSpace * 5 ) / 4;
	float pix4let = m_window.getSize().x * 0.025f;
	auto pos = sf::Vector2f(m_middle.x, getTitlePosY() + buttonSpace * 2.5f);
	makeBut<ChooseBoardState>(pos, singlePlayer, butHeight, pix4let, buttonSpace);
	makeBut<MultiplayerMenuState>(pos, multiplayer, butHeight, pix4let, buttonSpace);
	makeBut<HowToPlayState>(pos, help, butHeight, pix4let, buttonSpace);
	float width= Resources::getResourceRef().getButLen(ext) * pix4let;
	m_buttons.emplace_back(std::make_unique<BaseButton>(ext, pos, width, butHeight, true));
	Resources::getResourceRef().playMusic(menu);
};