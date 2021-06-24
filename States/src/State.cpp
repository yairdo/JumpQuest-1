#include "State.h"
#include "MainMenuState.h"
//-----------------------------------------------------------------------------
/*
	c-tor
*/
State::State(StateManager& machine, sf::RenderWindow& window, const bool replace,
	std::shared_ptr<NetworkObject> networkObj)
	: m_manager(machine), m_window(window), m_replacing(replace),
	m_networkObj(networkObj), m_paused(false){}
//-----------------------------------------------------------------------------
/*
	Function next:
	This function is return the next state into state manager
*/
std::unique_ptr<State> State::next()
{
	return std::move(m_next);
}
//-----------------------------------------------------------------------------
/*
	Function is replacing:
	This function return the raplce bollean to know if the state needs to be
	replaces or not.	
*/
bool State::isReplacing() const
{
	return m_replacing;
}
//-----------------------------------------------------------------------------
/*
	Function reset next:
	This function is reset the next state to main menu if there is exeption. 
*/
void State::resetNext() {
	m_next = m_manager.build<MainMenuState>(m_manager, m_window, true, nullptr);;
}

