#pragma once
#include "StaticObj.h"

class b2World;

class Gift : public StaticObj {
public:
	Gift(b2World& , const sf::Vector2f& , const sf::Vector2f&, int,int);
	virtual ~Gift() =default;
	bool collisionCounter();
	virtual void MsgCollision();

private:
	int m_counter = 3;
	static bool m_registerit;
};