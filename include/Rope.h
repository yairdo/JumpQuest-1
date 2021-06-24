#pragma once
#include "StaticObj.h"

class b2World;

class Rope : public StaticObj {
public:
	Rope(b2World&, const sf::Vector2f&, const sf::Vector2f&, int,int);
	virtual ~Rope() = default;
private:
	static bool m_registerit;
};
