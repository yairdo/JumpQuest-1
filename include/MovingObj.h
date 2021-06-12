#pragma once
#include "GameObj.h"
#include <Macros.h>

class MovingObj : public GameObj {
public:
	MovingObj(b2World& world, const sf::Vector2f& pos, const sf::Vector2f& size, int bodyType,int textNum=blank)
		: GameObj(world, pos, size, bodyType,textNum){}
	virtual ~MovingObj() = default;
	virtual void updatePhysics(float) = 0;
	virtual void draw(sf::RenderWindow&) = 0;
	virtual void move() = 0;

	/*virtual void move();
	virtual void handleCollision();*/

};