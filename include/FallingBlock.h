#pragma once
#include "MovingObj.h"

class b2World;

class FallingBlock : public MovingObj {
public:
	FallingBlock(b2World&, const sf::Vector2f&, const sf::Vector2f&, const sf::Vector2f&, int, int);
	virtual ~FallingBlock() = default;
	FallingBlock() = default;
	virtual void updatePhysics(float);
	virtual void move();
	virtual void updateAnim(float deltaTime);
	virtual MovingObjInfo getInfo() const { return MovingObjInfo(getPos(), m_activeTimer, m_body->GetLinearVelocity(),m_activeAnim); }
	virtual void setInfo(const MovingObjInfo& info);
	void setActiveAnim();
	virtual void reset() override;
private:
	bool m_activeAnim;
	sf::Vector2f m_strtPos;
	bool m_falling;
	float m_startingTime;
	static bool m_registerit;

};