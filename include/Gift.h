#pragma once
#include "StaticObj.h"

class b2Body;
class b2World;

class Gift : public StaticObj {
public:
	Gift(b2World& , const sf::Vector2f& , const sf::Vector2f&, int,int);
	virtual ~Gift();
	Gift()=default;
	virtual void draw(sf::RenderWindow&);
	bool collisionCounter();
	virtual void MsgCollision();
	virtual bool remove();
	//virtual void handleCollision();
	/*virtual void handleCol(GameObj* obj) override;
	virtual void handleGiftCol(Gift*) override {};
	void handlePlayerCol(Player* b);*/

private:
	//sf::RectangleShape m_testRect;
	int m_counter = 3;
	static bool m_registerit;
};