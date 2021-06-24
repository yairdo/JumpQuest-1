#pragma once
#include <MovingObj.h>
#include <Projectile.h>
#include <SFML/Graphics.hpp>

class Archer : public MovingObj {
public:
	//-----------c-tor------------
	Archer(b2World& world, const sf::Vector2f& pos,const sf::Vector2f& timerNDis,
		const sf::Vector2f& toPos, int bodyType,int mapEnum);
	//-----------d-tor------------
	virtual ~Archer() = default;

	//---------Functions----------
	virtual void updateAnim(float) override;
	virtual void updatePhysics(float) override;
	virtual void move() override;
	virtual MovingObjInfo getInfo()const override;
	virtual void setInfo(const MovingObjInfo&)override;

	virtual void draw(sf::RenderWindow& window);
private:
	
	//----------Members-----------
	int m_faceTo;
	float m_distance;
	float m_timer;
	sf::Vector2f m_shotStartPos;
	sf::Vector2f m_shotTO;
	std::unique_ptr<Projectile> m_proj;
	static bool m_registerit;
};