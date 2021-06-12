#pragma once
#include <SFML/Graphics.hpp>
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
	//updateAnim(int row, int col, float width, float height);
private:
	//Singeltone class
	sf::IntRect m_animRect;
	//std::vector<int> m_imageCount;
	//sf::Vector2u m_textSize;
	sf::Vector2u m_currImage;
	float m_totalTime;
	float m_switchTime;
	Animation();
	Animation(const Animation&) = default;
	Animation& operator=(const Animation&) = default;
	sf::Texture createTexture(const std::string&) const;
};