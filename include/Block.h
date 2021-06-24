#pragma once
#include "StaticObj.h"

class b2World;

class Block : public StaticObj {
public:
	//-----------c-tor------------
	Block(b2World&, const sf::Vector2f&, const sf::Vector2f&, int,int);

	//-----------d-tor------------
	virtual ~Block() = default;

	//---------Functions----------
	void updateAnim(float deltaTime);
	
private:
	//----------Members-----------
	static bool m_registerit;
};

