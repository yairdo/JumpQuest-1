#pragma once
#include "MovingObj.h"
#include "iostream"

class b2Body;
class b2World;
//class sf::RenderWindow;

class FallingObj : public MovingObj {
public:
	FallingObj(b2World&, const sf::Vector2f&, const sf::Vector2f&, int,int);
	virtual ~FallingObj() = default;
	FallingObj() = default;
	virtual void updatePhysics(float);
	virtual void move();
	virtual void draw(sf::RenderWindow&);
	virtual void updateAnim(float deltaTime);
	virtual MovingObjInfo getInfo() const { return movingObjInfoCreator(getPos(), m_timer, m_body->GetLinearVelocity()); }
	virtual void setInfo(MovingObjInfo info);
	void setActiveAnim();
	virtual void reset() override;

private:
	bool m_activeAnim;
	sf::Vector2f m_strtPos;
	bool m_falling = false;
	float m_timer = 3;
	static bool m_registerit;
};