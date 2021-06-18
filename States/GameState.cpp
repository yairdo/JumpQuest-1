#include <GameState.h>
#include <Player.h>
#include <StateManager.h>
#include <MainMenuState.h>
#include "Server.h"
#include <iostream>
#include <Projectile.h>

GameState::GameState(StateManager& manager, sf::RenderWindow& window, bool replace, std::shared_ptr<NetworkObject> net) :
	State(manager, window, replace, net), m_board(std::make_unique<Board>()),
	m_world(b2Vec2(0, 9.8)), /*m_isPlay(true),*/ m_deltaTime(1)/*, m_isServer(false), m_lastUpdate(0)*/
{
	
	/*if (m_networkObj && typeid(Server).name() == typeid(*m_networkObj.get()).name())
		m_isServer = true;*/



	m_backGround.setTexture(Resources::getResourceRef().getTexture(castle));
	
	m_backGround.setScale(0.35,window.getSize().y / m_backGround.getGlobalBounds().height);
	m_world.SetContactListener(&m_contactListner);
	m_board->generateMap(m_world);
	sf::Vector2f viewSize(m_window.getSize().x / 2, m_window.getSize().y);
	m_view = sf::View(sf::Vector2f(viewSize.x / 2, viewSize.y / 2), viewSize);
	m_view.setViewport({ 0.f,0.f,1,1 });
	addBorders2World();

	//m_testOtherPlayer = new sf::RectangleShape({ 50,50 });
	//m_testOtherPlayer->setOrigin(25, 25);
	//m_testOtherPlayer->setFillColor(sf::Color::Yellow);
	m_testPlayer = m_board->getPlayerRef();
	/*if (m_networkObj) {
		if (!m_networkObj->launch())
			m_isPlay = false;
	}*/


	//if (m_networkObj) {
	//	m_networkObj->setBoard(m_board.get());
	//	//test!!! test !!! test!!!
	//	for (int i = 0; i < MAX_SERVER_PLAYERS; ++i) {
	//		if (m_networkObj->getMembers(i) && m_networkObj->getInfo().m_info.m_id != m_networkObj->getMembers(i)->m_info.m_id) {
	//			m_clones[m_networkObj->getMembers(i)->m_info.m_id] = struct ClonePlayer();
	//		}
	//	}
	//}

	////------projectile test-------
	//m_testProjectile = new Projectile(m_world, {50, 100}, {10, 10}, b2_dynamicBody);
	////---------------------
	m_clock.restart();

}

void GameState::pause()
{
}

void GameState::resume()
{
}

void GameState::update()
{
	for (auto event = sf::Event{}; m_window.pollEvent(event);) {
		switch (event.type)
		{
		case sf::Event::Closed:
			m_manager.quit();
			break;

		case sf::Event::KeyPressed:
			switch (event.key.code)
			{
			case sf::Keyboard::Escape:
				m_manager.quit();
				break;

			case sf::Keyboard::M://maybe pause menue option
				//m_next = StateMachine::build<MenuState>(m_machine, m_window, false);
				break;

			default:
				break;
			}
			break;

		default:
			break;
		}
	}
	//if (!m_isPlay)//if failed to connect with the network object
	//{
	//	m_next = StateManager::build<MainMenuState>(m_manager, m_window, true, nullptr);
	//	return;
	//}

	updateGame();
	//if (m_isServer || !m_networkObj)
	//	updateServerGame();


	//else
	//	updateClientGame();

}
//void GameState::updateServerGame() {
//	//check if all players are ready
//	m_world.Step(TIME_STEP, VEL_ITERS, POS_ITERS);/////////////////////////////
//	if (m_clock.getElapsedTime().asSeconds() >= 0.001f)////////////////////////
//	{
//		m_deltaTime = m_clock.restart().asSeconds();////////////////////////
//
//
//		//if(m_networkObj)
//		//	m_networkObj->handleRequests(50);
//
//
//		//m_lastUpdate += m_deltaTime;
//		/////change to member and use reserve
//		//std::vector<MovingObjInfo> vec;
//		//if (m_networkObj && m_lastUpdate >= 0.03) {
//		//	for (int i = 1; i < m_board->numOfMovingObjs(); ++i) {
//		//		vec.push_back(m_board->getInfo(i));
//		//	}
//		//	((Server*)m_networkObj.get())->sendNewInfo(vec);
//		//	m_lastUpdate = 0;
//		//	m_clock.restart().asSeconds();
//		//}
//
//
//		updateBoard();
//
//
//		//if (m_networkObj) {
//		//	sendInfo(); ////////////////////////
//		//	updateClonesLoc();////////////////////////
//		//}
//
//
//		viewMover();////////////////////////
//		m_window.setView(m_view);////////////////////////
//		m_testPlayer->updateAnim(m_deltaTime);////////////////////////
//		m_board->updateBoard(m_networkObj.get());////////////////////////
//		m_deltaTime = m_clock.restart().asSeconds();
//		
//		////------test projectile
//		//if (!m_testProjectile->getShot())
//		//	m_testProjectile->shot(m_testProjectile->getPos(), {200, 100});
//		//std::cout << m_testProjectile->getPos().x << " " << m_testProjectile->getPos().y << std::endl;
//		//m_testProjectile->updatePhysics(m_deltaTime);
//		//m_testProjectile->move();
//		////------
//		//m_board->updatePhysics(m_deltaTime);
//		//m_board->move();
//		//if (m_networkObj) {
//		//	sendInfo();
//		//	updateClonesLoc();
//		//}
//		//viewMover();
//		//m_window.setView(m_view);
//		//m_testPlayer->updateAnim(m_deltaTime);
//		//m_board->updateBoard(m_networkObj.get());
//	}
//}

//void GameState::updateClientGame() {
//	m_world.Step(TIME_STEP, VEL_ITERS, POS_ITERS);////////////////////////
//	if (m_clock.getElapsedTime().asSeconds() >= 0.001f)////////////////////////
//	{
//
//
//		//m_networkObj->handleRequests(300);
//
//
//
//		m_deltaTime = m_clock.restart().asSeconds();////////////////////////
//		
//		updateBoard();
//		//end of text
//
//
//		//update animation???
//		//sendInfo();////////////////////////
//		//updateClonesLoc();////////////////////////
//
//
//		viewMover();////////////////////////
//		m_window.setView(m_view);////////////////////////
//		m_testPlayer->updateAnim(m_deltaTime);////////////////////////
//		m_board->updateBoard(m_networkObj.get());////////////////////////
//	}
//
//}


void GameState::draw()
{
	m_window.draw(m_backGround);
	m_board->draw(m_window);

	
	/*for (auto clone : m_clones)
		m_window.draw(clone.second.m_sprite);*/
	//m_window.draw(*m_testOtherPlayer);
	//for (auto clone : m_clones)
	//	m_window.draw(clone.second.m_sprite);
	//m_window.draw(*m_testOtherPlayer);
	//m_testProjectile->draw(m_window);
}
//-----------------------------------------------------------------------------
/*
	moves the view according to player location
*/
void GameState::viewMover() {
	sf::Vector2f playerPos = m_testPlayer->getPos();
	if (playerPos.x - m_window.getSize().x / 4 > 0)//we need here + player bounds width /2
		m_view.setCenter(playerPos.x, m_view.getCenter().y);
	//need to add boundries
}
//-----------------------------------------------------------------------------
/*
	adds edges to the physics world
*/
void GameState::addBorders2World() {
	auto screenSize = m_window.getSize();
	float widthInMeters = screenSize.x / SCALE;
	float heightInMeters = screenSize.y / SCALE;
	b2Vec2 topLeftCorner = b2Vec2(0, 0);
	b2Vec2 topRightCorner = b2Vec2(35, 0);
	b2Vec2 lowerLeftCorner = b2Vec2(0, heightInMeters);
	b2Vec2 lowerRightCorner = b2Vec2(35, heightInMeters);

	// static container body, with the collisions at screen borders
	b2BodyDef screenBorderDef;
	screenBorderDef.position.Set(0, 0);
	b2Body* screenBorderBody = m_world.CreateBody(&screenBorderDef);
	b2EdgeShape screenBorderShape;

	b2FixtureDef fixture;
	fixture.shape = &screenBorderShape;
	fixture.friction = 1.f;
	fixture.filter.categoryBits = boundryBits;

	// Create fixtures for the four borders (the border shape is re-used)
	screenBorderShape.Set(topLeftCorner, topRightCorner);
	screenBorderBody->CreateFixture(&fixture);
	screenBorderShape.Set(lowerRightCorner, topRightCorner);
	screenBorderBody->CreateFixture(&fixture);
	screenBorderShape.Set(lowerLeftCorner, topLeftCorner);
	screenBorderBody->CreateFixture(&fixture);
	screenBorderShape.Set(lowerRightCorner, lowerLeftCorner);
	screenBorderBody->CreateFixture(&fixture);
}

//void GameState::updateClonesLoc() {
//	for (int i = 0; i < MAX_SERVER_PLAYERS; ++i) {
//		if (m_networkObj->getMembers(i) && 
//			m_networkObj->getInfo().m_info.m_id != m_networkObj->getMembers(i)->m_info.m_id) {
//			auto info= m_networkObj->getMembers(i)->m_info;
//			//auto = struct of the info coming back from the server
//			//update map about the players new parameters from the info struct above
//			auto it = m_clones.find(m_networkObj->getMembers(i)->m_info.m_id);
//			if (it == m_clones.end())
//				continue;
//			it->second.m_sprite.setPosition(info.m_loc);
//			it->second.m_row = info.m_row;
//			it->second.m_col = info.m_col;
//			it->second.m_direction = info.m_direction;
//		}
//	}
//}
//
//void GameState::sendInfo() {
//	MemberInfo info = m_networkObj->getInfo().m_info;
//	info.m_row = m_testPlayer->getAnimRow();
//	info.m_col = m_testPlayer->getAnimCol();
//	info.m_loc = m_testPlayer->getPos();
//	info.m_direction = m_testPlayer->getDirection();
//	m_networkObj->updateLoc(info);
//}

void GameState::updateGame() {
	m_world.Step(TIME_STEP, VEL_ITERS, POS_ITERS);
	if (m_clock.getElapsedTime().asSeconds() >= 0.001f)
	{
		m_deltaTime = m_clock.restart().asSeconds();
		updateBoard();
		viewMover();
		m_window.setView(m_view);
		m_board->updateBoard(m_networkObj.get());
	}
}
//void GameState::updateClonesLoc() {
//	for (int i = 0; i < MAX_SERVER_PLAYERS; ++i) {
//		if (m_networkObj->getMembers(i) && m_networkObj->getInfo().m_info.m_id != m_networkObj->getMembers(i)->m_info.m_id) {
//			auto info = m_networkObj->getMembers(i)->m_info;
//			//auto = struct of the info coming back from the server
//			//update map about the players new parameters from the info struct above
//			auto it = m_clones.find(m_networkObj->getMembers(i)->m_info.m_id);
//			if (it == m_clones.end())
//				continue;
//			it->second.m_sprite.setPosition(info.m_loc);
//			it->second.m_row = info.m_row;
//			it->second.m_col = info.m_col;
//			it->second.m_direction = info.m_direction;
//		}
//	}
//
//}

void GameState::updateBoard(){
	m_board->updatePhysics(m_deltaTime);
	m_board->move();
}

b2World& GameState::getWorldRef(){
	return m_world;
}
