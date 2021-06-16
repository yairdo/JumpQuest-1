#pragma once
#include "GameObj.h"
#include <Macros.h>
class StaticObj : public GameObj {
public:
	StaticObj(b2World& world, const sf::Vector2f& pos, const sf::Vector2f& size, int bodyType,int textNum=blank)
		: GameObj(world, pos, size, bodyType,textNum),m_collision(false) {}
	StaticObj() = default;
	virtual ~StaticObj() = default;
	bool getCollision() const  { return m_collision; }
	void setCollision(bool collision) { m_collision = collision; }
	//virtual void handleCollision();
private:
	bool m_collision;
};