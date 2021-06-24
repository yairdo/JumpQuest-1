#pragma once
#include "GameObj.h"
class StaticObj : public GameObj {
public:
	StaticObj(b2World& world, const sf::Vector2f& pos, const sf::Vector2f& size, int bodyType,
			  float recWidth=0.f, float recHeight=0.f, int textNum=blank,int mapEnum=castle)
			  : GameObj(world, pos, size, bodyType,recWidth,recHeight,textNum,mapEnum) {}
	virtual ~StaticObj() = default;
};