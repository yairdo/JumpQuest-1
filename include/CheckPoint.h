#pragma once
#include "StaticObj.h"

class b2World;

class CheckPoint : public StaticObj {
public:
	
	//----------c-tor-------------
	CheckPoint(b2World&, const sf::Vector2f&, const sf::Vector2f&,
		const sf::Vector2f&, int,int);
	CheckPoint() = default;

	//----------d-tor-------------
	virtual ~CheckPoint() = default;

	//---------Functions----------
	void activate();
	bool getActive() const;
	void updateAnim(float deltaTime);
	bool getWin() const;
private:
	//----------Members-----------
	bool m_win;
	static bool m_registerit;
	bool m_activate;
};