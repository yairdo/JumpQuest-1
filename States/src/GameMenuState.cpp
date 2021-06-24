#include "GameMenuState.h"
#include "MainMenuState.h"
#include "BaseButton.h"
#include <memory>
#include "Macros.h"
//-----------------------------------------------------------------------------
/*
	c-tor
*/
GameMenuState::GameMenuState(StateManager& manager, sf::RenderWindow& window,
	bool replace):
	StandardMenuState(manager,window,replace)
{
	m_buttonSpace = m_window.getSize().y * BUTTUN_SPACE_PREC;
	m_butHeight = (m_window.getSize().y/ BUT3 - SPACES2 *m_buttonSpace) / BUT3;
	m_pix4let = m_window.getSize().x * PREC4LET;
	m_pos = sf::Vector2f(m_middle.x, (window.getSize().y - m_butHeight* BUT3)/ SPACES2);
	float width = Resources::getResourceRef().getButLen(TexturesNum::resume) * m_pix4let;
	m_buttons.emplace_back(std::make_unique<BaseButton>(TexturesNum::resume,
		m_pos, width, m_butHeight, false));
	m_pos.y += float(m_buttonSpace + m_butHeight);
	makeBut<MainMenuState>(m_pos, mainMenu, m_butHeight, m_pix4let, m_buttonSpace);
	width = Resources::getResourceRef().getButLen(ext) * m_pix4let;
	m_buttons.emplace_back(std::make_unique<BaseButton>(ext, m_pos, width, m_butHeight, true));
}
//-----------------------------------------------------------------------------
/*
	Fuinction: update next state
	This function is updating the next state of each button while it pressed
*/
void GameMenuState::updateNextState(const sf::Vector2f& loc) {
	if (m_buttons[FIRST_BUT]->checkCollision(loc)) {
		m_manager.lastState();
	}
	if (m_buttons[SEC_BUT]->checkCollision(loc)) {
		m_next = m_buttons[SEC_BUT]->ButtonState(m_manager, m_window, true, m_networkObj);
	}
	if (m_buttons[THIRD_BUT]->checkCollision(loc))
		m_manager.quit();

}
//-----------------------------------------------------------------------------
/*
	Function: update
	This function is updating the game state on the background, the buttons 
	positions for the view and itself.
*/
void GameMenuState::update() {
	
	m_manager.updateLastState();
	updateButtonsPos();
	MenuState::update();
}
//-----------------------------------------------------------------------------

void GameMenuState::draw() {
	m_manager.drawLastState();
	MenuState::draw();
}
//-----------------------------------------------------------------------------
/*
	Function update buttons pos
	This function is updating the position of the buttons each iterate because
	of the moving view.
*/
void GameMenuState::updateButtonsPos() {
	for (auto& but : m_buttons) {
		but->setPos(m_window.getView().getCenter().x);
	}
}
