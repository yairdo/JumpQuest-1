#pragma once

#include "MovingObj.h"
#include "iostream"
class b2Body;
class b2World;
//class sf::RenderWindow;

class Projectile : public MovingObj {
public:
	Projectile(b2World&, const sf::Vector2f&, const sf::Vector2f&, const sf::Vector2f&, int);
	virtual ~Projectile() = default;

	virtual void updatePhysics(float);
	virtual void move();
	virtual void shot();
	virtual void draw(sf::RenderWindow&);

	void reset();

	sf::Vector2f getPos();

private:
	sf::Vector2f m_strtPos;
	bool m_shot = false;
	sf::Vector2f m_force;
};