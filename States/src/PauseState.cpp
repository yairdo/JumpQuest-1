#include "PauseState.h"
#include "MainMenuState.h"
#include "BaseButton.h"
#include <memory>
#include "Macros.h"

PauseState::PauseState(StateManager& manager, sf::RenderWindow& window, bool replace):
	StandardMenuState(manager,window,replace){
	auto buttonSpace = m_window.getSize().y * 0.05;
	auto butHeight = (m_window.getSize().y/3-2*buttonSpace) / 3;
	float pix4let = m_window.getSize().x * 0.022;
	auto pos = sf::Vector2f(m_middle.x, (window.getSize().y - butHeight*3)/2);
	makeBut<MainMenuState>(pos, mainMenu, butHeight, pix4let, buttonSpace);
	float width = Resources::getResourceRef().getButLen(TexturesNum::resume) * pix4let;
	m_buttons.emplace_back(std::make_unique<BaseButton>(TexturesNum::resume,pos,width,butHeight,false));
	pos.y += buttonSpace + butHeight;
	width = Resources::getResourceRef().getButLen(ext) * pix4let;
	m_buttons.emplace_back(std::make_unique<BaseButton>(ext, pos, width, butHeight, true));
}
void PauseState::updateNextState(const sf::Vector2f& loc) {
	if (m_buttons[0]->checkCollision(loc)) {
		m_next = m_buttons[0]->ButtonState(m_manager, m_window, true, m_networkObj);
	}
	if (m_buttons[1]->checkCollision(loc)) {
		m_manager.lastState();
	}
	if (m_buttons[2]->checkCollision(loc))
		m_manager.quit();

}
void PauseState::update() {
	m_manager.updateLastState();
	updateButtonsPos();
	MenuState::update();
	auto evnt = sf::Event{}.key.code;
	if(evnt==sf::Keyboard::Escape){
		m_manager.lastState();
	}
	
}

void PauseState::draw() {
	m_manager.drawLastState();
	MenuState::draw();
}
void PauseState::updateButtonsPos() {
	for (auto& but : m_buttons) {
		but->setPos(m_window.getView().getCenter().x);
	}
}
