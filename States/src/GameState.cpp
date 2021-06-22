#include <GameState.h>
#include <Player.h>
#include <StateManager.h>
#include <MainMenuState.h>
#include "Server.h"
#include <iostream>
#include <Projectile.h>
#include "PauseState.h"
#include <thread>


GameState::GameState(StateManager& manager, sf::RenderWindow& window, bool replace,
	std::shared_ptr<NetworkObject> net, int map) :
	State(manager, window, replace, net), m_board(std::make_unique<Board>()),
	m_world(b2Vec2(0, 9.8)), m_deltaTime(1) {

	//m_testProjectile = new Projectile(getWorldRef(), PROJECTILE_SIZE, b2_dynamicBody);
	m_backGround.setTexture(Resources::getResourceRef().getTexture(map, gameBackground));
	Resources::getResourceRef().playMusic(map);
	m_backGround.setScale(0.6, window.getSize().y / m_backGround.getGlobalBounds().height);
	m_world.SetContactListener(&m_contactListner);
	int id = (net == nullptr) ? 0 : net->getInfo().m_info.m_id;
	m_board->setId(id);
	m_board->setmapEnum(map);
	m_board->generateMap(m_world);

	sf::Vector2f viewSize(m_window.getSize().x / 2, m_window.getSize().y);
	m_view = sf::View(sf::Vector2f(viewSize.x / 2.f, viewSize.y / 2.f), viewSize);
	m_view.setViewport({ 0.f,0.f,1,1 });
	addBorders2World();
	m_testPlayer = m_board->getPlayerRef();
	if (net) m_testPlayer->setName(net->getInfo().m_name);
	m_clock.restart();

}

void GameState::pause()
{
	m_paused = true;
	auto evnt = sf::Event{};
	while (m_window.pollEvent(evnt));
}

void GameState::resume()
{
	m_paused = false;
	auto evnt = sf::Event{};
	while (m_window.pollEvent(evnt));
}

void GameState::update()
{	
	if (!m_paused) {
		for (auto event = sf::Event{}; m_window.pollEvent(event);) {
			switch (event.type)
			{
			case sf::Event::Closed:
				m_manager.quit();
				break;

			case sf::Event::KeyReleased:
				switch (event.key.code)
				{
				case sf::Keyboard::Escape:
					m_next = std::make_unique<PauseState>(m_manager, m_window, false);
					break;

					//case sf::Keyboard::M://maybe pause menue option
					//	//m_next = StateMachine::build<MenuState>(m_machine, m_window, false);
					//	break;

				default:
					break;
				}
				break;

			default:
				break;
			}
		}
	}


	updateGame();
	if (m_testPlayer->getWin()) {
		win();//show win title for 2 seconds and back to main menu
		return;
	}

}


void GameState::draw()
{
	
	m_window.draw(m_backGround);
	m_board->draw(m_window);
	
	//m_testProjectile->draw(m_window);
}
//-----------------------------------------------------------------------------
/*
	moves the view according to player location
*/
void GameState::viewMover() {
	sf::Vector2f playerPos = m_testPlayer->getPos();
	if (playerPos.x - m_window.getSize().x / 4 < 0){
		m_view.setCenter(m_window.getSize().x / 4, m_view.getCenter().y);
	}
	else if ((playerPos.x + m_window.getSize().x / 4) > MAP_SIZE){
	m_view.setCenter(MAP_SIZE- m_window.getSize().x / 4, m_view.getCenter().y);
	}
	//if (playerPos.x - m_window.getSize().x / 4 > 0 && (playerPos.x + m_window.getSize().x / 4) < MAP_SIZE)
	else
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
	b2Vec2 topRightCorner = b2Vec2(60, 0);
	b2Vec2 lowerLeftCorner = b2Vec2(0, heightInMeters);
	b2Vec2 lowerRightCorner = b2Vec2(60, heightInMeters);

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

void GameState::setView(const sf::View& view) {
	m_view = std::move(view);
}

void GameState::updateGame() {
	m_world.Step(TIME_STEP, VEL_ITERS, POS_ITERS);
	if (m_clock.getElapsedTime().asSeconds() >= 0.001f)
	{
		m_deltaTime = m_clock.restart().asSeconds();
		viewMover();
		updateBoard();
		
		m_window.setView(m_view);
		m_board->updateBoard(m_networkObj.get());
	}


	//-----------------------------------------------------
	static float projTimer = 3;
	projTimer -= m_deltaTime;
	//for (auto evnt = sf::Event{}; m_window.pollEvent(evnt);) {
	//	if (evnt.type == sf::Event::MouseButtonReleased) {
	if (!m_paused && sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
		m_testPlayer->useGift(m_window.mapPixelToCoords(sf::Mouse::getPosition()), m_networkObj.get());
		//m_testProjectile->shot(m_testProjectile->getPos(), m_window.mapPixelToCoords(sf::Mouse::getPosition()));
		//std::cout << "shot\n";
		//m_testProjectile->setShot(false);
	}
}

void GameState::updateBoard(){
	m_board->updatePhysics(m_deltaTime);
	m_board->move();
}

b2World& GameState::getWorldRef(){
	return m_world;
}

void GameState::win() {
	sf::Text text;
	text.setFont(Resources::getResourceRef().getFont(lobbyFont));
	text.setString("YOU WIN");
	text.setCharacterSize(100);
	text.setLetterSpacing(2.f);
	text.setFillColor(sf::Color::White);
	text.setOutlineColor(sf::Color::Black);
	text.setOutlineThickness(5.f);
	text.setOrigin(text.getGlobalBounds().width / 2.f, text.getGlobalBounds().height / 2.f);
	text.setPosition(m_window.getView().getCenter());
	m_window.draw(text);
	m_window.display();
	using namespace std::chrono_literals;
	std::this_thread::sleep_for(5s);
	m_next= std::make_unique<MainMenuState>(m_manager, m_window, true);
}
