#pragma once
#include "GameObj.h"
#include<MessagesStructs.h>

class MovingObj : public GameObj {
public:
	MovingObj(b2World& world, const sf::Vector2f& pos, const sf::Vector2f& size, int bodyType,
		float recWidth=0.f, float recHeight=0.f, int textNum=blank, int mapEnum=castle)
		: GameObj(world, pos, size, bodyType,recWidth,recHeight,textNum,mapEnum){}
	MovingObj() = default;
	virtual ~MovingObj() = default;
	virtual void updatePhysics(float) = 0;
	virtual void move() = 0;

	virtual MovingObjInfo getInfo()const  { return MovingObjInfo(getPos(), 0, m_body->GetLinearVelocity()); }
	virtual void setInfo(const MovingObjInfo& info) { setPos(info.m_loc); }

	bool getReset() const { return m_reset; }
	void setReset(bool reset) { m_reset = reset; }

	virtual void reset() {};
protected :
	float m_activeTimer;
private:
	bool m_reset = false;

};