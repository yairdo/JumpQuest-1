#pragma once
#include "StaticObj.h"

class b2Body;
class b2World;

class Block : public StaticObj {
public:
	Block(b2World&, const sf::Vector2f&, const sf::Vector2f&, int);
	virtual ~Block() = default;
	virtual void draw(sf::RenderWindow&);
};
