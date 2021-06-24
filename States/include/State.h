#pragma once
#include <memory>
#include <NetworkObject.h>
#include <SFML/Graphics.hpp>
#include <Resources.h>
class StateManager;

namespace sf
{
	class RenderWindow;
}

class State
{
public:
	//-----------c-tor------------

	State(StateManager& machine, sf::RenderWindow& window, bool replace = true, std::shared_ptr<NetworkObject> = nullptr);

	//-----------d-tor------------

	virtual ~State() = default;

	
	State(const State&) = delete;
	State& operator=(const State&) = delete;

	//---------Functions----------
	virtual void pause() = 0;
	virtual void resume() = 0;

	virtual void update() = 0;
	virtual void draw() = 0;

	virtual void resetNext();

	std::unique_ptr<State> next();

	bool isReplacing() const;
	

protected:
	//----------Members-----------
	StateManager& m_manager;
	sf::RenderWindow& m_window;
	bool m_paused;
	bool m_replacing;
	std::shared_ptr<NetworkObject> m_networkObj;
	std::unique_ptr<State> m_next;
};