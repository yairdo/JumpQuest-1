#include <GameState.h>
#include <Player.h>
#include <StateManager.h>
#include <MainMenuState.h>
#include "Server.h"
#include <iostream>
#include <Projectile.h>
#include "GameMenuState.h"

//-----------------------------------------------------------------------------
/*
	c-tor
*/
GameState::GameState(StateManager& manager, sf::RenderWindow& window, bool replace,
	std::shared_ptr<NetworkObject> net, int map) :
	State(manager, window, replace, net), m_board(std::make_unique<Board>()),
	m_world(b2Vec2(0, 9.8)), m_deltaTime(1), m_isWin(false),m_winTimer(0.f)
{
	m_backGround.setTexture(Resources::getResourceRef().getTexture(map, gameBackground));
	Resources::getResourceRef().playMusic(map);
	m_backGround.setScale(GAME_BACK, window.getSize().y / m_backGround.getGlobalBounds().height);
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
	setText(m_winnerText, 50, 1.f, sf::Color::White, sf::Color::Black, 5.f);
	setText(m_projectileActive, 20, 1.f, sf::Color::White, sf::Color::Black, 1.f);
	m_projectileActive->setString("LEFT CLICK TO SHOOT");

}
//-----------------------------------------------------------------------------
/*
	Function: pause:
	This function is "pausing" the game for the game menu.
	The game will continue updating at the background and the player can move.
*/
void GameState::pause() 
{
	m_paused = true;
}
//-----------------------------------------------------------------------------
/*
	Function: resume
	This function is resuming the game.
*/
void GameState::resume()
{
	m_paused = false;
}
//-----------------------------------------------------------------------------
/*
	Function: update
	This function is updating the game and handle with clicks.
*/
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
					m_next = std::make_unique<GameMenuState>(m_manager, m_window, false);
					break;
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
}

//-----------------------------------------------------------------------------

void GameState::draw()
{
	m_window.draw(m_backGround);
	m_board->draw(m_window);
	if (m_isWin) {
		m_winnerText->setPosition(m_window.getView().getCenter());
		m_window.draw(*m_winnerText.get());
	}
	if (m_testPlayer->getGotGift()) {
		m_projectileActive->setPosition({ m_window.getView().getCenter().x-m_projectileActive->getGlobalBounds().width/2, m_window.getSize().y - 50.f });
		m_window.draw(*m_projectileActive.get());
	}
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
	else
		m_view.setCenter(playerPos.x, m_view.getCenter().y);
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

//-----------------------------------------------------------------------------
/*
	This function is setting the view position.
*/
void GameState::setView(const sf::View& view) {
	m_view = std::move(view);
}

//-----------------------------------------------------------------------------
/*
	Function update game:
	This function is updating the game objects using delta time.
*/
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
	if (!m_paused && sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
		m_testPlayer->useGift(m_window.mapPixelToCoords(sf::Mouse::getPosition()),
			m_networkObj.get());
		m_testPlayer->setGotGift(false);
	}
	if (!m_isWin)
		updateWin();
	else {
		m_winTimer += m_deltaTime;
		if (m_winTimer >= 5.f)
			m_next = std::make_unique<MainMenuState>(m_manager, m_window, true);
	}
}
//-----------------------------------------------------------------------------
/*
	Function: update board: this function is updating the objects physics and
	sprites.
*/
void GameState::updateBoard(){
	m_board->updatePhysics(m_deltaTime);
	m_board->move();
}

//-----------------------------------------------------------------------------
/*
	Function: set text
	This function is setting the text's messages data.
*/
void GameState::setText(std::unique_ptr<sf::Text>& text, unsigned int size, float spacing,
						const sf::Color& fillColor, const sf::Color& outlineColor, float outline) {
	text = std::make_unique<sf::Text>();
	text->setFont(Resources::getResourceRef().getFont(lobbyFont));
	text->setCharacterSize(size);
	text->setLetterSpacing(spacing);
	text->setFillColor(fillColor);
	text->setOutlineColor(outlineColor);
	text->setOutlineThickness(outline);
}
//-----------------------------------------------------------------------------
/*
	Function update win
	This function is updating the win message and the text message.
*/
void GameState::updateWin() {
	if (!m_isWin && m_testPlayer->getWin()) {
		Resources::getResourceRef().playMusic(win);
		m_winnerText->setString("You Won!");
		m_winnerText->setOrigin(m_winnerText->getGlobalBounds().width / 2.f,
			m_winnerText->getGlobalBounds().height / 2.f);
		m_isWin = true;
	}
}
