#include "Controller.h"
#include "MainMenuState.h"
//#include "Macros.h"
//#include "Player.h"
//#include "Block.h"
//#include "MovingBlock.h"
//#include "FallingBlock.h"
//#include <iostream>
//#include <Gift.h>
//#include <Rope.h>
//
////#include "Client.h"
////#include "Server.h"
//
//Controller::Controller() : m_window(sf::RenderWindow
//	(sf::VideoMode(sf::VideoMode::getFullscreenModes()[0]), "Jump Quest",
//	sf::Style::Close | sf::Style::Titlebar)), m_deltaTime(1), m_isPlay(false)
//	, m_world(b2Vec2(0, 9.8)),
//	m_menuView({ 0.f,0.f,float(m_window.getSize().x),
//	float(m_window.getSize().y) }),
//	m_mainMenu(m_window.getSize(), { 1.5f,1.5f }, { singlePlayer ,multiplayer,help,ext },
//	menuBackground, title),
//	m_multiplayerMenu(m_window.getSize(),{1.5f,1.5f},{host,client,back},menuBackground,
//	title), m_isMain(true), m_contactListner(),
//	m_board(std::make_unique<Board>()){
//	m_world.SetContactListener(&m_contactListner);
//	//m_board = new Board();
//
//	//TEMP TEMP TEMPPPP - YAIR
//	//std::cout << "started genereating, for debugging\n";
//	m_board->generateMap(m_world);
//	//TEMP TEMP - YAIR
//
//	sf::Vector2f viewSize(m_window.getSize().x / 2, m_window.getSize().y);
//	m_view = sf::View(sf::Vector2f(viewSize.x / 2, viewSize.y / 2), viewSize);
//	m_view.setViewport({ 0.f,0.f,1,1 });
//	addBorders2World();
//	//create board.
//	//create menu.
//	m_netObj = nullptr;
//	/*m_server = nullptr;
//	m_client = nullptr;*/
//}
//
//void Controller::run() {
//	m_window.setFramerateLimit(FRAME_RATE);
//	//---for world time step
//	float timeStep = 1.0f / 60.0f;
//	int32 velocityIterations = 6;
//	int32 positionIterations = 2;
//	m_testPlayer = m_board->getPlayerRef();
//
//	while (m_window.isOpen()) {
//		//Temp!!!!!
//		if (!m_isPlay && m_netObj) {
//			if (m_netObj->run(m_window))
//				m_isPlay = true;//rungame
//			else
//				m_netObj.reset(nullptr); 
//			continue;
//		}
//		m_window.clear();
//		
//		if (!m_isPlay) {
//			m_window.setView(m_menuView);
//			runMenu();
//		}
//		else {
//			m_world.Step(timeStep, velocityIterations, positionIterations);
//			//for (auto& moving : m_testMoving) 
//			//	moving->updatePhysics(m_deltaTime);
//			m_board->updatePhysics(m_deltaTime);
//			//m_testPlayer->updatePhysics(m_deltaTime);
//			TempViewMover();
//			m_window.setView(m_view);
//			runGame();
//		}
//	}
//}
//
//void Controller::runGame() {
//	pollEvents();
//	//updateWindow();
//	if (m_netObj) /// it was m _sever
//		m_netObj; //same
//	render();
//}
//void Controller::render() {
//	//m_testPlayer->move();
//	//m_testPlayer->draw(m_window);
//	m_board->draw(m_window);
//	if (m_netObj) {
//		m_netObj->updateLoc(m_testPlayer->getPos(), 0);
//		m_netObj->handleRequests(10);
//		for (int i = 0; i < MAX_SERVER_PLAYERS; ++i) {
//			if (m_netObj->getMembers(i) && m_netObj->getInfo().m_id != m_netObj->getMembers(i)->m_id) {
//				sf::Vector2f loc = m_netObj->getMembers(i)->m_loc;
//				sf::RectangleShape shape({ 50, 50 });
//				shape.setOrigin(25, 25);
//				shape.setPosition(loc); 
//				shape.setFillColor(sf::Color::Yellow);
//				m_window.draw(shape); // all of this is temp!!
//			}
//		}
//	}
//
//	m_window.display();
//}
//
//void Controller::pollEvents() {
//	for (auto evnt = sf::Event{}; m_window.pollEvent(evnt);) {
//		switch (evnt.type)
//		{
//		case sf::Event::Closed:
//			m_window.close();
//			break;
//		case sf::Event::KeyPressed:
//			//auto dir = findDirection(evnt.key.code);
//			//send direction to player;
//			//move view;
//			/*if (evnt.key.code == sf::Keyboard::Space)
//				m_testPlayer->jump();*/
//			
//			break;
//		/*default:
//			break;*/
//		}
//	}
//}
////-----------------------------------------------------------------------------
//void Controller::runMenu() {
//	sf::Vector2f location{ 0,0 };
//	if (auto evnt = sf::Event{}; m_window.waitEvent(evnt)) {
//
//		switch (evnt.type) {
//		case::sf::Event::Closed:
//			m_window.close();
//			break;
//		case::sf::Event::MouseButtonReleased:
//			location = m_window.mapPixelToCoords
//			(sf::Vector2i(evnt.mouseButton.x, evnt.mouseButton.y));
//			switch (evnt.mouseButton.button) {
//			case sf::Mouse::Left:
//				handleMenuClick(location);
//				break;
//			}
//			break;
//		case::sf::Event::MouseMoved:
//			location = m_window.mapPixelToCoords
//			(sf::Vector2i(evnt.mouseMove.x, evnt.mouseMove.y));
//			(m_isMain) ? m_mainMenu.setTransp(location) : m_multiplayerMenu.setTransp(location);
//			break;
//		}
//	}
//	(m_isMain) ? m_mainMenu.draw(m_window) : m_multiplayerMenu.draw(m_window);
//	m_window.display();
//}
//
//void Controller::handleMenuClick(const sf::Vector2f& location) {
//	auto type = (m_isMain) ? m_mainMenu.checkCollision(location) : m_multiplayerMenu.checkCollision(location);
//	switch (type) {
//	case singlePlayer:
//		m_isPlay = true;
//		break;
//	case ext:
//		m_window.close();
//		break;
//	case multiplayer:
//		m_isMain = false;
//		break;
//	case back:
//		m_isMain = true;
//		break;
//	case help:
//		//open help;
//		break;
//	case host:
//		////temp!!!!!!!
//		m_isMain = true;
//		m_netObj = std::make_unique<Server>();
//		//m_server = std::make_unique<Server>();	//open host
//		break;
//		////temp!!!!!!!
//	case client:
//		m_isMain = true;
//		m_netObj = std::make_unique<Client>();
//		//m_client=std::make_unique<Client>();
//		//open client
//		break;
//	default:
//		break;
//	}
//}
///*
////	
////*/
////sf::Vector2f Controller::findDirection(sf::Keyboard::Key key) {
////	switch (key) {
////	case sf::Keyboard::Left:
////		return { -1,0 };
////	case sf::Keyboard::Right:
////		return { 1,0 };
////	case sf::Keyboard::Up:
////		return { 0,-1 };
////	case sf::Keyboard::Down:
////		return { 0,1 };
////	default:
////		return { 0,0 };
////
////	}
////}
////-----------------------------------------------------------------------------
///*
//	adds edges to the physics world
//*/
//void Controller::addBorders2World() {
//	auto screenSize = m_window.getSize();
//	float widthInMeters = screenSize.x / SCALE;
//	float heightInMeters = screenSize.y / SCALE;
//	b2Vec2 topLeftCorner = b2Vec2(0, 0);
//	b2Vec2 topRightCorner = b2Vec2(widthInMeters, 0);
//	b2Vec2 lowerLeftCorner = b2Vec2(0, heightInMeters);
//	b2Vec2 lowerRightCorner = b2Vec2(widthInMeters, heightInMeters);
//
//	// static container body, with the collisions at screen borders
//	b2BodyDef screenBorderDef;
//	screenBorderDef.position.Set(0, 0);
//	b2Body* screenBorderBody = m_world.CreateBody(&screenBorderDef);
//	b2EdgeShape screenBorderShape;
//
//	b2FixtureDef fixture2;
//	fixture2.shape = &screenBorderShape;
//	fixture2.friction = 1.f;
//	fixture2.filter.categoryBits = boundryBits;
//
//	// Create fixtures for the four borders (the border shape is re-used)
//	screenBorderShape.Set(topLeftCorner, topRightCorner);
//	screenBorderBody->CreateFixture(&fixture2);
//	screenBorderShape.Set(lowerRightCorner, topRightCorner);
//	screenBorderBody->CreateFixture(&fixture2);
//	screenBorderShape.Set(lowerRightCorner, lowerLeftCorner);
//	screenBorderBody->CreateFixture(&fixture2);
//	screenBorderShape.Set(lowerLeftCorner, topLeftCorner);
//	screenBorderBody->CreateFixture(&fixture2);
//}
//
//
//void Controller::TempViewMover() {
//	sf::Vector2f playerPos = m_testPlayer->getPos();
//	if(playerPos.x-m_window.getSize().x/4 >0)//we need here + player bounds width /2
//		m_view.setCenter(playerPos.x, m_view.getCenter().y);
//	//need to add boundries
//}//
//sf::VideoMode::getFullscreenModes()[0]


Controller::Controller() : m_window(sf::RenderWindow
	(sf::VideoMode(1920,1080), "Jump Quest",
	sf::Style::Close | sf::Style::Titlebar/* |sf::Style::Fullscreen*/))
{
	m_window.setFramerateLimit(FRAME_RATE);
}

void Controller::run(){

	m_manager.run(StateManager::build<MainMenuState>(m_manager, m_window, true, nullptr));

	while (m_manager.running())
	{
		m_window.clear();
		m_manager.nextState();
		m_manager.draw();
		m_manager.update();
		m_window.display();
	}
	m_window.close();
}