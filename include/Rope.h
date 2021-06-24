#pragma once
#include "StaticObj.h"

class b2Body;
class b2World;

class Rope : public StaticObj {
public:
	Rope(b2World&, const sf::Vector2f&, const sf::Vector2f&, int,int);
	Rope() = default;
	virtual ~Rope() = default;
	virtual void setSprite(float recWidth = 0, float recHeight = 0, const sf::Vector2f& size = { 0,0 },
		const sf::Vector2f& pos = { 0,0 })override;
private:
	static bool m_registerit;
};
