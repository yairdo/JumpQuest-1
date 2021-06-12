#include "MenuState.h"
#include <memory>
#include "Macros.h"
#include "Resources.h"

MenuState::MenuState(StateManager& manager,sf::RenderWindow& window,
	const bool replace ,std::shared_ptr<NetworkObject>& net,int title,
	int background):
	m_title(std::make_unique<sf::Sprite>(Resources::getResourceRef().getTexture(title))),
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
	setTitle();
}

void MenuState::draw(){
	//m_window.clear();
	m_window.draw(*(m_background.get()));
	m_window.draw(*(m_title.get()));
	std::for_each(m_buttons.begin(), m_buttons.end(),
	[&](const std::unique_ptr<BaseButton>& button) {button->draw(m_window); });
	//m_window.display();
}

void MenuState::setTransp(const sf::Vector2f& location) const{
	std::for_each(m_buttons.begin(), m_buttons.end(),
		[&](const std::unique_ptr<BaseButton>& but){but->setTransp(location); });
}
void MenuState::update(){
	auto location = sf::Vector2f{ 0,0 };
	for (auto event = sf::Event{}; m_window.pollEvent(event);) {
		switch (event.type) {
		case sf::Event::Closed:
				m_manager.quit();
				break;
		case::sf::Event::MouseButtonReleased:
			location = m_window.mapPixelToCoords
			(sf::Vector2i(event.mouseButton.x, event.mouseButton.y));
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
//void MenuState::setTitle(const sf::Vector2f& titleScale, 
//		const sf::Vector2f& loc, int title){
//	m_title = std::make_unique<sf::Sprite>(Resources::getResourceRef().getTexture(title));
//	m_title->setOrigin(m_title->getGlobalBounds().width / 2,
//		m_title->getGlobalBounds().height / 2);
//	m_title->setPosition(loc);
//	m_title->setScale(titleScale);
//}

void MenuState::setTitle(sf::Sprite&& title){
	m_title = std::make_unique<sf::Sprite>(title);
}

float MenuState::getTitleHeight() const
{
	return m_title->getGlobalBounds().height;
}

//void MenuState::addButton(int index, int type,
//		const sf::Vector2f& loc, const sf::Vector2f& size){
//	m_buttons.emplace_back(std::make_unique<Button>(m_menu->getStateRef(index), type, loc, size));
//}

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
void MenuState::setTitle() {
	
	m_title->setOrigin(m_title->getGlobalBounds().width / 2,
		m_title->getGlobalBounds().height / 2);
	m_title->setPosition(m_middle.x, m_title->getGlobalBounds().height);
	m_title->setScale({ 1.5f,1.5f });
}
