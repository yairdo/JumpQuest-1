#pragma once

#include "MovingObj.h"
#include "iostream"
#include <Animation.h>
#include <Projectile.h>
class b2World;
class sf::RenderWindow;

class Player : public MovingObj {
public:
	Player(b2World&, const sf::Vector2f&, const sf::Vector2f&, int);
	virtual ~Player()=default;

	//--added for tests
	void collectGift();
	virtual void updatePhysics(float);
	void jump(float);
	virtual void move();
	virtual void draw(sf::RenderWindow&);
	void setOnRope(bool);
	bool getOnRope() { return m_onRope; };

	//test
	void toggleCanCatch() { m_canCatch = !m_canCatch; };

	int getDirection();
	void footContact(int val);
	void updateAnim(float deltaTime);
	void setCheckPoint(const sf::Vector2f&);
	virtual void reset() override;
	void center(const sf::Vector2f&);
private:
	void updateRow();
	sf::Vector2f m_checkPoint;
	int m_id;
	int m_direction;
	bool m_onRope = false;
	//test
	bool m_canCatch = false;
	std::unique_ptr<Projectile> m_projectile;
	int m_lives;
	int m_numFootContact;
	sf::Vector2f m_offSet;
	//Animation m_anim;
};