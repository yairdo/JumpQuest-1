#pragma once
#include <SFML/Graphics.hpp>
#include "StateManager.h"
//#include <ctime>
//#include "Board.h"
//#include "box2d/box2d.h"
//#include "MovingObj.h"
//#include "Menu.h"
//#include "ContactListner.h"
//#include <memory>
//
//#include "Client.h"
//#include "Server.h"
//#include "GameObj.h"

//class Player;

class Controller {
public:
	Controller();
	void run();

private:
	StateManager m_manager;
	sf::RenderWindow m_window;
	//Board* m_board;
	//std::unique_ptr<Board> m_board;
	//sf::RenderWindow m_window;
	//Menu m_mainMenu;
	//Menu m_multiplayerMenu;
	//b2World m_world;
	//sf::Clock m_clock;
	//float m_deltaTime;
	//sf::View m_view;
	//sf::View m_menuView;
	//bool m_isPlay;
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
	//void TempViewMover();
	//--physics stuff

	//void addBorders2World();
	//ContactListner m_contactListner;
	//Player* m_testPlayer;
	//std::vector<std::unique_ptr<GameObj>> m_testStatic;
	//std::vector<std::unique_ptr<MovingObj>> m_testMoving;

	//TEMP!!!
	//std::unique_ptr<NetworkObject> m_netObj;
	//std::unique_ptr<Client> m_client;
	//std::unique_ptr<Server> m_server;
};