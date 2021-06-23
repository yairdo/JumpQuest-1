#pragma once

#include "MovingObj.h"
#include "iostream"

class b2Body;
class b2World;
class sf::RenderWindow;

class MovingBlock : public MovingObj {
public:
	MovingBlock(b2World&, const sf::Vector2f&, const sf::Vector2f& endPos, const sf::Vector2f&, int,int);
	virtual ~MovingBlock() = default;
	MovingBlock() = default;

	//--added for tests
	virtual void updatePhysics(float);
	virtual void move();
	virtual void draw(sf::RenderWindow&);
	b2Vec2 getVel() const { return m_body->GetLinearVelocity(); }
	float getWidth();
private:
	sf::Vector2f m_strtPos;
	sf::Vector2f m_endPos;
	static bool m_registerit;
};