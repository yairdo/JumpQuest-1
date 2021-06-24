#pragma once
#include <State.h>
#include <Board.h>
#include "box2d/box2d.h"
#include "ContactListner.h"


class Player;
class sf::RectangleShape;
class Projectile;

class GameState : public State {
public:
	//-----------c-tor------------
	GameState(StateManager&, sf::RenderWindow&, bool replace = true,
		std::shared_ptr<NetworkObject> = nullptr,int=castle);

	//-----------d-tor------------
	virtual ~GameState() = default;
	
	//----------Functions---------
	virtual void pause();
	virtual void resume();
	virtual void update();
	virtual void draw();
	
private:
	//-----------Members----------
	sf::Sprite m_backGround;
	ContactListner m_contactListner;
	b2World m_world;
	sf::View m_view;

	//------Private Functions-----

	void viewMover();
	void updateGame();
	float m_winTimer;
	void addBorders2World();

protected:
	//-----------Members----------

	Player* m_testPlayer;
	float m_deltaTime;
	sf::Clock m_clock;
	std::unique_ptr<Board> m_board;
	bool m_isWin;
	std::unique_ptr<sf::Text> m_projectileActive;
	std::unique_ptr<sf::Text> m_winnerText;
	
	//-----Protected Functions----

	virtual void updateBoard();

	virtual void updateWin();
	void setView(const sf::View&);
	void setText(std::unique_ptr<sf::Text>&, unsigned int, float,const sf::Color&,
		const sf::Color&, float);
};