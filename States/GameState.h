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
	GameState(StateManager&, sf::RenderWindow&, bool replace = true, std::shared_ptr<NetworkObject> = nullptr);
	//state functions
	virtual void pause();
	virtual void resume();
	virtual void update();
	//-----test for server
	//void updateServerGame();
	//void updateClientGame();
	//----
	virtual void draw();
	//game functions
	void addBorders2World();

private:
	sf::Sprite m_backGround;
	
	ContactListner m_contactListner;
	
	//float m_lastUpdate; //temp
	sf::RectangleShape* m_testOtherPlayer;
	//view stuff
	sf::View m_view;
	void viewMover();
	void updateGame();

protected:
	b2World m_world;
	Player* m_testPlayer;
	float m_deltaTime;
	sf::Clock m_clock;
	std::unique_ptr<Board> m_board;
	virtual void updateBoard();
	b2World& getWorldRef();
};