#pragma once

#include "MovingObj.h"
#include "iostream"

class b2Body;
class b2World;
//class sf::RenderWindow;

class FallingBlock : public MovingObj {
public:
	FallingBlock(b2World&, const sf::Vector2f&, const sf::Vector2f&, int);
	virtual ~FallingBlock() = default;
	FallingBlock() = default;
	virtual void updatePhysics(float);
	virtual void move();
	virtual void draw(sf::RenderWindow&);

	void reset();

	sf::Vector2f getPos();

private:
	sf::Vector2f m_strtPos;
	bool m_falling = false;
	float m_timer = 3;
	static bool m_registerit;
};