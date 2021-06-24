#pragma once
#include "MovingObj.h"

class b2World;
class sf::RenderWindow;

class MovingBlock : public MovingObj {
public:
	MovingBlock(b2World&, const sf::Vector2f&, const sf::Vector2f& endPos, const sf::Vector2f&, int,int);
	virtual ~MovingBlock() = default;

	virtual void updatePhysics(float);
	virtual void move();
	//---getters
	b2Vec2 getVel() const { return m_body->GetLinearVelocity(); }
	float getWidth() const;
private:
	sf::Vector2f m_strtPos;
	sf::Vector2f m_endPos;
	static bool m_registerit;
};