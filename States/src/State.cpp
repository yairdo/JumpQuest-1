#include "State.h"
//-----------------------------------------------------------------------------
State::State(StateManager& machine, sf::RenderWindow& window, const bool replace, std::shared_ptr<NetworkObject> networkObj)
	: m_manager(machine), m_window(window), m_replacing(replace), m_networkObj(networkObj), m_paused(false)
{}
//-----------------------------------------------------------------------------
std::unique_ptr<State> State::next()
{
	return std::move(m_next);
}
//-----------------------------------------------------------------------------
bool State::isReplacing() const
{
	return m_replacing;
}