#pragma once
#include "StaticObj.h"

class b2Body;
class b2World;

class Rope : public StaticObj {
public:
	Rope(b2World&, const sf::Vector2f&, const sf::Vector2f&, int);
	virtual ~Rope() = default;
	virtual void draw(sf::RenderWindow&);
};