#pragma once
#include <SFML/Graphics.hpp>

class Animation {
public:
	//----------Functions---------
	static Animation& getAnimRef();
	const sf::IntRect updateAnim(int row, int& col, float deltaTime,
		float& totalTime, int type,int dir, float switchTime=0.15f);
private:
	//Singeltone class

	//----------Members-----------
	sf::IntRect m_animRect;
	int m_width;
	int m_height;

	//------Private Functions-----
	int findLineLen(int obejct, int row) const;
	void setWidthHeight(int type);

	//-------c-tor, copy c-tor..--
	Animation() = default;
	Animation(const Animation&) = default;
	Animation& operator=(const Animation&) = default;
};