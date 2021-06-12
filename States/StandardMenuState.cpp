#include "StandardMenuState.h"
#include "Resources.h"
#include "Macros.h"
#include "StateManager.h"


//StandardMenuState::StandardMenuState(StateManager& manager, sf::RenderWindow& window,
//		bool replace, std::shared_ptr<NetworkObject> net, int title):
//	MenuState(manager,window,replace,net,menuBackground),
//	m_middle(m_window.getSize().x / 2.f, m_window.getSize().y / 2.f)
//{	
//
//	sf::Sprite titlesp (Resources::getResourceRef().getTexture(title));
//	titlesp.setOrigin(titlesp.getGlobalBounds().width / 2,
//		titlesp.getGlobalBounds().height / 2);
//	titlesp.setPosition(m_middle.x, titlesp.getGlobalBounds().height);
//	titlesp.setScale({ 1.5f,1.5f });
//	//auto loc=
//	setTitle(std::move(titlesp));
//}
