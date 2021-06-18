#pragma once
#include "State.h"

#include <memory>
#include <stack>

class NetworkObject;
namespace sf {
	class RenderWindow;
}
class StateManager
{
public:
	StateManager();

	void run(std::unique_ptr<State> state);

	void nextState();
	void lastState();

	void update();
	void draw();

	bool running() const;
	void quit();

	template <typename T>
	static std::unique_ptr<T> build(StateManager& machine, sf::RenderWindow& window, bool replace = true, std::shared_ptr<NetworkObject> netObj = nullptr);

private:
	// The stack of states
	std::stack<std::unique_ptr<State>> m_states;

	bool m_resume;
	bool m_running;
};

template <typename T>
std::unique_ptr<T> StateManager::build(StateManager& machine, sf::RenderWindow& window, bool replace, std::shared_ptr<NetworkObject> netObj)
{
	return std::make_unique<T>(machine, window, replace,netObj);
}