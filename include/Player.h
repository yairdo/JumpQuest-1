#pragma once

#include "MovingObj.h"
#include "iostream"
#include <Animation.h>
class b2World;
class sf::RenderWindow;

class Player : public MovingObj {
public:
	Player(b2World&, const sf::Vector2f&, const sf::Vector2f&, int);
	virtual ~Player()=default;

	//--added for tests
	virtual void updatePhysics(float);
	void jump(float);
	virtual void move();
	virtual void draw(sf::RenderWindow&);
	void setOnRope(bool);
	bool getOnRope() { return m_onRope; };
	int getDirection();
	void footContact(int val);
	void updateAnim(float deltaTime);
	void setCheckPoint(const sf::Vector2f&);
	virtual void reset() override;
private:
	void updateRow();
	sf::Vector2f m_checkPoint;
	int m_direction;
	bool m_onRope = false;
	int m_lives;
	int m_numFootContact;
	//Animation m_anim;
};