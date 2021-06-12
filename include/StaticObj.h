#pragma once
#include "GameObj.h"
#include <Macros.h>
class StaticObj : public GameObj {
public:
	StaticObj(b2World& world, const sf::Vector2f& pos, const sf::Vector2f& size, int bodyType,int textNum=blank)
		: GameObj(world, pos, size, bodyType,textNum) {}
	virtual ~StaticObj() = default;
	//virtual void handleCollision();
private:

};