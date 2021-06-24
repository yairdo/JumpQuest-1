#pragma once

#include "MovingObj.h"
#include "iostream"

class b2Body;
class b2World;

class Projectile : public MovingObj {
public:
	Projectile(b2World&, const sf::Vector2f&, int,float,int=0);
	virtual ~Projectile() = default;
	virtual void updatePhysics(float);
	virtual void move();
	virtual void shot(const sf::Vector2f&);
	sf::Vector2f getPosToShotFrom(const sf::Vector2f&, const sf::Vector2f&, const sf::Vector2f&);
	bool getShot() { return m_shot; };
	void reset();
	void setShot(bool);
	float getDis()  const;
	void setDis(float);
	sf::Vector2f getPos() const;
	b2Vec2 getForce(sf::Vector2f) const;
	void setFace(int);
	void updateAnim(float deltaTime);
	void setPosition(const sf::Vector2f& loc);
	void setInfo(const MovingObjInfo& info);
private:
	bool m_activeAnim;
	int m_faceTo;
	float m_distance;
	bool m_shot;
	b2Vec2 m_vel;
	float m_elapaseTime;
	bool m_applied = false;
};