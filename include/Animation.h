#pragma once
#include <SFML/Graphics.hpp>
#include <Macros.h>
//class Animation {
//public:
//	const sf::IntRect update(int row, float deltaTime);
//	Animation(float width, float height, sf::Texture& ,std::vector<short>);
//	Animation() = default;
//private:
//	sf::IntRect m_animRect;
//	std::vector<int> m_imageCount;
//	sf::Vector2u m_textSize;
//	sf::Vector2u m_currImage;
//	float m_totalTime;
//	float m_switchTime;
//};
class Animation {
public:
	static Animation& getAnimRef();
	const sf::IntRect updateAnim(int row, int& col, float deltaTime, float& totalTime, int type,int dir);
private:
	//Singeltone class
	sf::IntRect m_animRect;
	int m_width;
	int m_height;
	//std::vector<int> m_imageCount;
	//sf::Vector2u m_textSize;
	//sf::Vector2u m_currImage;
	int findLineLen(int obejct, int row);
	void setWidthHeight(int type);
	Animation() = default;
	Animation(const Animation&) = default;
	Animation& operator=(const Animation&) = default;
};