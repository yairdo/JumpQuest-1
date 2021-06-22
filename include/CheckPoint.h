#pragma once

#include "StaticObj.h"

class b2Body;
class b2World;

class CheckPoint : public StaticObj {
public:
	CheckPoint(b2World&, const sf::Vector2f&, const sf::Vector2f&, const sf::Vector2f&, int,int);
	CheckPoint() = default;
	virtual ~CheckPoint() = default;
	virtual void draw(sf::RenderWindow&);
	void setColor(sf::Color color);
	void activate();
	bool getActive() const;
	void updateAnim(float deltaTime);
	bool getWin() const;
private:
	bool m_win;
	static bool m_registerit;
	bool m_activate;
};