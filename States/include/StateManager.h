#pragma once
#include "State.h"

#include <memory>
#include <stack>

class NetworkObject;
class sf::RenderWindow;

class StateManager
{
public:
	
	//-----------c-tor------------
	StateManager(sf::RenderWindow&);

	//---------Functions----------
	void run(std::unique_ptr<State> state);

	void nextState();
	void lastState();

	void update();
	void draw();

	bool running() const;
	void quit();

	void updateLastState();
	void drawLastState();
	bool isLast() const;
	template <typename T>
	static std::unique_ptr<T> build(StateManager& machine, sf::RenderWindow& window,
		bool replace = true, std::shared_ptr<NetworkObject> netObj = nullptr);
	void setErrorMessage(const std::string&);
	void setStateManagerText();


private:

	//----------Members-----------
	// The stack of states
	std::stack<std::unique_ptr<State>> m_states;
	State* m_lastState;
	bool m_resume;
	bool m_running;

	float m_errorMessageTimer;
	sf::Text m_errorMessage;
	sf::RenderWindow& m_window;
};
//-----------------------------------------------------------------------------
/*
	Function: build
	This function is building new state.
*/
template <typename T>
std::unique_ptr<T> StateManager::build(StateManager& machine, sf::RenderWindow& window,
	bool replace, std::shared_ptr<NetworkObject> netObj)
{
	return std::make_unique<T>(machine, window, replace,netObj);
}
