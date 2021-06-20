#include <StateManager.h>

StateManager::StateManager(): m_resume( false ), m_running( false ){}

void StateManager::run(std::unique_ptr<State> state)
{
	m_running = true;
	m_states.push(std::move(state));
}

void StateManager::nextState()
{
    if (m_resume)
    {
        // Cleanup the current state
        if (!m_states.empty())
        {
            m_states.pop();
        }

        // Resume previous state
        if (!m_states.empty())
        {
            m_states.top()->resume();
        }

        m_resume = false;
    }

	// There needs to be a state
	if (!m_states.empty())
	{
		auto temp = m_states.top()->next();

		// Only change states if there's a next one existing
		if (temp != nullptr)
		{
			// Replace the running state
			if (temp->isReplacing())
			{
				while (!m_states.empty())
					m_states.pop();
			}
			// Pause the running state
			else
			{
				m_lastState = m_states.top().get();
				m_states.top()->pause();
			}

			m_states.push(std::move(temp));
		}
	}
}

void StateManager::lastState()
{
    m_resume = true;
}

void StateManager::update()
{
	// Let the state update the game
	m_states.top()->update();
}

void StateManager::draw()
{
	m_states.top()->draw();
}

bool StateManager::running() const
{
	return m_running;
}

void StateManager::quit()
{
	m_running = false;
}

void StateManager::updateLastState() {
	m_lastState->update();
}
void StateManager::drawLastState() {
	m_lastState->draw();
}
