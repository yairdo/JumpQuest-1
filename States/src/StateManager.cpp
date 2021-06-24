#include <StateManager.h>
#include <Macros.h>
//-----------------------------------------------------------------------------
/*
	c-tor
*/
StateManager::StateManager(sf::RenderWindow& window): m_window(window), m_resume( false ),
	m_running( false ),m_lastState(nullptr), m_errorMessageTimer(0){}
//-----------------------------------------------------------------------------
/*
	Function run:
	this function is runing the current state.
*/
void StateManager::run(std::unique_ptr<State> state)
{
	m_running = true;
	m_states.push(std::move(state));
}
//-----------------------------------------------------------------------------
/*
	Function: next state
	This function is updating and set the next state.
*/
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
				m_lastState = nullptr;
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
//-----------------------------------------------------------------------------
/*
	Function: is last
	This function checking if there is state inside the stack before the 
	current state. 
*/
bool StateManager::isLast() const {
	return m_lastState;
}
//-----------------------------------------------------------------------------
/*
	Function last state:
	This function is noting the last state to continue.
*/
void StateManager::lastState()
{
    m_resume = true;
}
//-----------------------------------------------------------------------------
/*
	Function update:
	This function is runing the update of the current state. 
*/

void StateManager::update()
{
	try {
		m_states.top()->update();
	}
	catch(const std::exception& e){
		m_states.top()->resetNext();
		m_window.setView(m_window.getDefaultView());
		setErrorMessage(e.what());
	}
}
//-----------------------------------------------------------------------------

void StateManager::draw()
{
	m_states.top()->draw();
	if (m_errorMessageTimer > 0) {
		m_window.draw(m_errorMessage);
		m_errorMessageTimer -= 0.01f;
	}
}
//-----------------------------------------------------------------------------
bool StateManager::running() const
{
	return m_running;
}
//-----------------------------------------------------------------------------
/*
	Function quit:
	This function quit the program by updating the running bool
*/
void StateManager::quit()
{
	m_running = false;
}
//-----------------------------------------------------------------------------
/*
	This function is updating the last state (for game menu state).
*/
void StateManager::updateLastState() {
	m_lastState->update();
}
//-----------------------------------------------------------------------------
/*
	Tis function is draw the last state (for game menu state).
*/
void StateManager::drawLastState() {
	m_lastState->draw();
}
//-----------------------------------------------------------------------------
/*
	Function: set error message 
	This function is setting the string of the error message (for exeption)
*/
void StateManager::setErrorMessage(const std::string& str) {
	m_errorMessage.setString(str);
	m_errorMessage.setPosition({ m_window.getSize().x / 2.f - 
		m_errorMessage.getGlobalBounds().width / 2.f,EXEPTION_HEIGHT});
	m_errorMessageTimer = 3;
}
//-----------------------------------------------------------------------------
/*
	Function: set state manager text
	This function is setting the text's template for the error message
*/
void StateManager::setStateManagerText() {
	m_errorMessage.setFont(Resources::getResourceRef().getFont(lobbyFont));
	m_errorMessage.setCharacterSize(50);
	m_errorMessage.setString("");
	m_errorMessage.setFillColor(sf::Color::White);
	m_errorMessage.setOutlineColor(sf::Color::Black);
	m_errorMessage.setOutlineThickness(1.f);
}
