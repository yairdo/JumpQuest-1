#pragma once
#include "MovingObj.h"
#include "iostream"

class b2Body;
class b2World;
//class sf::RenderWindow;

class FloorObstacle : public MovingObj {
public:
	FloorObstacle(b2World&, const sf::Vector2f&, const sf::Vector2f&, const sf::Vector2f&, int, int);
	virtual ~FloorObstacle() = default;
	FloorObstacle() = default;
	virtual void updatePhysics(float);
	virtual void move();
	virtual void draw(sf::RenderWindow&);
	virtual void updateAnim(float deltaTime);
	virtual MovingObjInfo getInfo() const { return MovingObjInfo(getPos(), m_timer, m_body->GetLinearVelocity()); }
	virtual void setInfo(MovingObjInfo info);
	virtual void reset() override;

private:
	bool m_active;
	float m_timer;
	float m_startingTime;
	static bool m_registerit;
	sf::Vector2f m_size;

};