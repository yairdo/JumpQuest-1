#pragma once
#include "MovingObj.h"

class b2World;

class FloorObstacle : public MovingObj {
public:
	FloorObstacle(b2World&, const sf::Vector2f&, const sf::Vector2f&, const sf::Vector2f&, int, int);
	virtual ~FloorObstacle() = default;
	virtual void updatePhysics(float);
	virtual void move() {}
	virtual void updateAnim(float deltaTime) {}
	virtual MovingObjInfo getInfo() const { return MovingObjInfo(getPos(), m_timer, m_body->GetLinearVelocity()); }
	virtual void setInfo(const MovingObjInfo& info) {}
	bool getActive() const;


private:
	bool m_active;
	float m_timer;
	float m_currTimer;
	int m_currIndex;
	float m_scaler;
	static bool m_registerit;
	sf::Vector2f m_size;
	sf::Vector2f m_currentSize;
};