#include "PauseState.h"
#include "MainMenuState.h"
#include "BaseButton.h"
#include <memory>
#include "Macros.h"
//-----------------------------------------------------------------------------
PauseState::PauseState(StateManager& manager, sf::RenderWindow& window, bool replace):
	StandardMenuState(manager,window,replace){
	auto buttonSpace = m_window.getSize().y * BUTTUN_SPACE_PREC;
	auto butHeight = (m_window.getSize().y/ BUT3 - SPACES2 *buttonSpace) / BUT3;
	float pix4let = m_window.getSize().x * PREC4LET;
	auto pos = sf::Vector2f(m_middle.x, (window.getSize().y - butHeight* BUT3)/ SPACES2);
	float width = Resources::getResourceRef().getButLen(TexturesNum::resume) * pix4let;
	m_buttons.emplace_back(std::make_unique<BaseButton>(TexturesNum::resume, pos, width, butHeight, false));
	pos.y += float(buttonSpace + butHeight);
	makeBut<MainMenuState>(pos, mainMenu, butHeight, pix4let, buttonSpace);
	width = Resources::getResourceRef().getButLen(ext) * pix4let;
	m_buttons.emplace_back(std::make_unique<BaseButton>(ext, pos, width, butHeight, true));
}
//-----------------------------------------------------------------------------
void PauseState::updateNextState(const sf::Vector2f& loc) {
	if (m_buttons[FIRST_BUT]->checkCollision(loc)) {
		m_manager.lastState();
	}
	if (m_buttons[SEC_BUT]->checkCollision(loc)) {
		m_next = m_buttons[FIRST_BUT]->ButtonState(m_manager, m_window, true, m_networkObj);
	}
	if (m_buttons[THIRD_BUT]->checkCollision(loc))
		m_manager.quit();

}
//-----------------------------------------------------------------------------
void PauseState::update() {
	
	m_manager.updateLastState();
	updateButtonsPos();
	MenuState::update();

}
//-----------------------------------------------------------------------------
void PauseState::draw() {
	m_manager.drawLastState();
	MenuState::draw();
}
//-----------------------------------------------------------------------------
void PauseState::updateButtonsPos() {
	for (auto& but : m_buttons) {
		but->setPos(m_window.getView().getCenter().x);
	}
}
