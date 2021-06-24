#pragma once

#include "MovingObj.h"
#include "iostream"
#include <Animation.h>
#include <Projectile.h>
#include <Board.h>
class b2World;
class NetworkObject;
class sf::RenderWindow;

class Player : public MovingObj {
public:
	Player(b2World&, const sf::Vector2f&, const sf::Vector2f&, int, int , Board& board);
	virtual ~Player()=default;

	//--added for tests
	void collectGift();
	virtual void updatePhysics(float);
	void jump(float);
	virtual void move();
	virtual void draw(sf::RenderWindow&);
	void setOnRope(bool);
	bool getOnRope() { return m_onRope; };
	void useGift(const sf::Vector2f&, NetworkObject*);
	void setGotGift(bool state){ m_gotGift = state; }
	bool getGotGift() const { return m_gotGift; }
	void setExternalForce(const b2Vec2&);
	bool getMoving()const { return m_moving; }
	void setMoving(bool vel);
	//sf::Vector2f getPosToShotFrom(sf::Vector2f);

	void toggleCanCatch() { m_canCatch = !m_canCatch; };
	int getDirection();
	void footContact(int val);
	void updateAnim(float deltaTime);
	void setCheckPoint(const sf::Vector2f&);
	virtual void reset() override;
	void center(const sf::Vector2f&);
	void setName(const std::string&);
	void winGame();
	bool getWin() const { return m_win; }
	float getWidth();
	void setPush(bool push) { m_pushDown = push; }
private:
	bool m_pushDown;
	sf::Text m_name;
	void updateRow();
	sf::Vector2f m_checkPoint;
	int m_id;
	int m_direction;
	bool m_onRope;
	//test
	bool m_canCatch;
	Board* m_board;
	bool m_gotGift;
	int m_lives;
	int m_numFootContact;
	sf::Vector2f m_offSet;
	b2Vec2 m_projectileForce;
	bool m_win;
	bool m_moving;
	bool m_stuned;
	float m_stunTime;

	//Animation m_anim;
};