#include <GameState.h>
#include <Player.h>
#include <StateManager.h>
#include <MainMenuState.h>
#include "Server.h"
#include <iostream>
#include <Projectile.h>

GameState::GameState(StateManager& manager, sf::RenderWindow& window, bool replace, std::shared_ptr<NetworkObject> net) :
	State(manager, window, replace, net), m_board(std::make_unique<Board>()),
	m_world(b2Vec2(0, 9.8)),  m_deltaTime(1)
{

	m_backGround.setTexture(Resources::getResourceRef().getTexture(castle));
	
	m_backGround.setScale(0.35,window.getSize().y / m_backGround.getGlobalBounds().height);
	m_world.SetContactListener(&m_contactListner);
	m_board->generateMap(m_world);
	sf::Vector2f viewSize(m_window.getSize().x / 2, m_window.getSize().y);
	m_view = sf::View(sf::Vector2f(viewSize.x / 2.f, viewSize.y / 2.f), viewSize);
	m_view.setViewport({ 0.f,0.f,1,1 });
	addBorders2World();
	m_testPlayer = m_board->getPlayerRef();
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

	updateGame();

}


void GameState::draw()
{
	m_window.draw(m_backGround);
	m_board->draw(m_window);
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

	// Create fixtures for the four borders (the border shape is re-used)
	screenBorderShape.Set(topLeftCorner, topRightCorner);
	screenBorderBody->CreateFixture(&fixture);
	screenBorderShape.Set(lowerRightCorner, topRightCorner);
	screenBorderBody->CreateFixture(&fixture);
	screenBorderShape.Set(lowerRightCorner, lowerLeftCorner);
	screenBorderBody->CreateFixture(&fixture);
	screenBorderShape.Set(lowerLeftCorner, topLeftCorner);
	screenBorderBody->CreateFixture(&fixture);
	fixture.filter.categoryBits = boundryBits;
	screenBorderShape.Set(lowerRightCorner, lowerLeftCorner);
	screenBorderBody->CreateFixture(&fixture);
}

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

void GameState::updateBoard(){
	m_board->updatePhysics(m_deltaTime);
	m_board->move();
}

b2World& GameState::getWorldRef(){
	return m_world;
}