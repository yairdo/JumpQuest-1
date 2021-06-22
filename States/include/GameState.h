
#pragma once
#include <State.h>
#include <Board.h>
#include "box2d/box2d.h"
#include "ContactListner.h"
//#include <unordered_map>

class Player;
class sf::RectangleShape;
class Projectile;

class GameState : public State {
public:
	GameState(StateManager&, sf::RenderWindow&, bool replace = true, std::shared_ptr<NetworkObject> = nullptr,int=castle);
	//state functions
	virtual void pause();
	virtual void resume();
	virtual void update();
	virtual void draw();
	
	//game functions
	
	
	

private:
	sf::Sprite m_backGround;

	ContactListner m_contactListner;
	Projectile* m_testProjectile;
	//view stuff
	sf::View m_view;
	void viewMover();
	void updateGame();
	float m_winTimer;
	void addBorders2World();

protected:
	b2World m_world;
	Player* m_testPlayer;
	float m_deltaTime;
	sf::Clock m_clock;
	std::unique_ptr<Board> m_board;
	virtual void updateBoard();
	b2World& getWorldRef();
	bool m_isWin;
	std::unique_ptr<sf::Text> m_winnerText;
	void setWinText();
	virtual void updateWin();
	void setView(const sf::View&);
};