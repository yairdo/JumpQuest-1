#pragma once
#include <StaticObj.h>
#include <Projectile.h>
#include <SFML/Graphics.hpp>

class Archer : public StaticObj {
public:
	Archer(b2World& world, const sf::Vector2f& pos,
		const sf::Vector2f& timerNDis, const sf::Vector2f& toPos, int bodyType,int mapEnum);
	virtual ~Archer() = default;
	void updateAnim(float deltaTime);
	virtual void draw(sf::RenderWindow& window);
private:
	float m_distance;
	float m_timer;
	sf::Vector2f m_shotStartPos;
	sf::Vector2f m_shotTO;
	std::unique_ptr<Projectile> m_proj;
	static bool m_registerit;
};