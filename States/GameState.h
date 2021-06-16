#pragma once
#include <State.h>
#include <Board.h>
#include "box2d/box2d.h"
#include "ContactListner.h"
#include <unordered_map>

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
	void updateServerGame();
	void updateClientGame();
	//----
	virtual void draw();
	//game functions
	void addBorders2World();

private:
	sf::Sprite m_backGround;
	
	b2World m_world;
	ContactListner m_contactListner;
	
	
	
	float m_lastUpdate; //temp
	sf::RectangleShape* m_testOtherPlayer;
	//view stuff
	sf::View m_view;
	void viewMover();
	bool m_isServer;
	std::unordered_map<int,struct ClonePlayer> m_clones;
	//void updateClonesLoc();
	//void sendInfo();
	//sf::View m_menuView;
	bool m_isPlay;
	Projectile* m_testProjectile;
	//bool m_isMain;
	//std::unique_ptr<Board> m_board;
	//void runMenu();
	//void runGame();
	//void update();
	//void pollEvents();
	//void render();
	//void handleClick();
	//void print();
	//sf::Vector2f findDirection(sf::Keyboard::Key key);
	//void moveView(const sf::Vector2f& dir);
	//void handleMenuClick(const sf::Vector2f& location);

	//TEMP TEMP TEMP
	//--physics stuff

	//void addBorders2World();
	//std::vector<std::unique_ptr<GameObj>> m_testStatic;
	//std::vector<std::unique_ptr<MovingObj>> m_testMoving;

	//TEMP!!!
	//std::unique_ptr<NetworkObject> m_netObj;
	//std::unique_ptr<Client> m_client;
	//std::unique_ptr<Server> m_server;

	void updateGame();

protected:
	Player* m_testPlayer;
	float m_deltaTime;
	sf::Clock m_clock;
	std::unique_ptr<Board> m_board;
	virtual void updateBoard();
};