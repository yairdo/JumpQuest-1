#include "MenuState.h"
#include <memory>
#include "Macros.h"
#include "Resources.h"

//-----------------------------------------------------------------------------
MenuState::MenuState(StateManager& manager,sf::RenderWindow& window,
	const bool replace ,std::shared_ptr<NetworkObject> net,int titl,
	int background):
	m_title(std::make_unique<sf::Sprite>(Resources::getResourceRef().getTexture(titl))),
	State{ manager, window, replace,net } ,
	m_middle(m_window.getSize().x / 2.f, m_window.getSize().y / 2.f),
	m_background(std::make_unique<sf::Sprite>(Resources::getResourceRef().
	getTexture(background)))
{
	//m_middle(m_window.getSize().x / 2.f, m_window.getSize().y / 2.f)
	auto x = m_background->getGlobalBounds().width;
	auto y = m_background->getGlobalBounds().height;
	m_background->setScale(float(window.getSize().x) / x, float(window.getSize().y) / y);
	m_background->setPosition({ 0.f,0.f });
	setTitle(titl);
}
//-----------------------------------------------------------------------------
MenuState::MenuState(StateManager& manager, sf::RenderWindow& window, bool replace):
	State(manager,window,replace),
	m_middle(m_window.getView().getSize().x / 2.f, m_window.getView().getSize().y / 2.f){}

//-----------------------------------------------------------------------------
void MenuState::draw(){
	if (m_background)
		m_window.draw(*(m_background.get()));
	if (m_title)
		m_window.draw(*(m_title.get()));
	std::for_each(m_buttons.begin(), m_buttons.end(),
	[&](const std::unique_ptr<BaseButton>& button) {button->draw(m_window); });
}
//-----------------------------------------------------------------------------
void MenuState::setTransp(const sf::Vector2f& location) const{
	std::for_each(m_buttons.begin(), m_buttons.end(),
		[&](const std::unique_ptr<BaseButton>& but){but->setTransp(location); });
}
//-----------------------------------------------------------------------------
void MenuState::update(){
	auto location = sf::Vector2f{ 0,0 };
	for (auto event = sf::Event{}; m_window.pollEvent(event);) {
		switch (event.type) {
		case sf::Event::Closed:
				m_manager.quit();
				break;
		case sf::Event::KeyReleased:
			if (event.key.code == sf::Keyboard::Escape && m_manager.isLast())
				m_manager.lastState();
			break;
		case::sf::Event::MouseButtonReleased:
			location = m_window.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, 
															  event.mouseButton.y));
			switch (event.mouseButton.button) {
			case sf::Mouse::Left:
				updateNextState(location);
				break;
			}
			break;
		case::sf::Event::MouseMoved:
			location = m_window.mapPixelToCoords
			(sf::Vector2i(event.mouseMove.x, event.mouseMove.y));
			setTransp(location);
			break;
		}
	}
}
//-----------------------------------------------------------------------------
void MenuState::setTitle(sf::Sprite&& title){
	m_title = std::make_unique<sf::Sprite>(title);
}
//-----------------------------------------------------------------------------
float MenuState::getTitleHeight() const
{
	//return m_title->getGlobalBounds().height;
	return m_title->getPosition().y;
}
//-----------------------------------------------------------------------------
float MenuState::getTitlePosY() const
{
	return (m_title->getPosition().y+m_title->getGlobalBounds().height/2);
}
//-----------------------------------------------------------------------------


void MenuState::updateNextState(const sf::Vector2f& loc){
	for (auto& but: m_buttons){
		if (but->checkCollision(loc)) {
			if (but->getIsExit())
				m_manager.quit();
			else
				m_next = but->ButtonState(m_manager, m_window, true, m_networkObj);
			break;
		}
	}
}
//-----------------------------------------------------------------------------
void MenuState::setTitle(int titl) {
	
	m_title->setOrigin(m_title->getGlobalBounds().width / 2,
		m_title->getGlobalBounds().height / 2);
	m_title->setPosition(m_middle.x, m_window.getSize().y*0.15);
	m_title->setScale({ Resources::getResourceRef().getButLen(titl)*
		m_window.getSize().x*0.07f/m_title->getGlobalBounds().width,
		m_window.getSize().y*0.25f/ m_title->getGlobalBounds().height });
}
