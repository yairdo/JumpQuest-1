#pragma once

#include "MovingObj.h"
#include "iostream"
class b2Body;
class b2World;
//class sf::RenderWindow;

class Projectile : public MovingObj {
public:
	Projectile(b2World&, const sf::Vector2f&, int,float);
	virtual ~Projectile() = default;

	virtual void updatePhysics(float);
	virtual void move();
	virtual void shot(const sf::Vector2f&, const sf::Vector2f&);
	virtual void draw(sf::RenderWindow&);
	bool getShot() { return m_shot; };
	void reset();
	void setShot(bool);
	float getDis()  const;
	sf::Vector2f getPos();

private:
	float m_distance;
	bool m_shot;
	b2Vec2 m_vel;
	float m_elapaseTime;
	bool m_applied = false;
};