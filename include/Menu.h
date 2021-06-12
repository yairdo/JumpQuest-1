//#pragma once
//#include <SFML/Graphics.hpp>
//#include <unordered_map>
//#include "Button.h"
//#include <vector>
//
//class Menu {
//public:
//	Menu(const sf::Vector2u& windowSize, const sf::Vector2f& titleScale,
//		const std::vector<int>& vec, int background, int title);
//	//Menu(const sf::Vector2u & = {0, 0});
//	void draw(sf::RenderWindow& window) const;
//	void setTransp(const sf::Vector2f&) ;
//	int checkCollision(const sf::Vector2f&) ;
//	//void handleCollision();
//
//private:
//	sf::Vector2f m_middle;
//	sf::Sprite m_background;
//	sf::Sprite m_title;
//	std::vector<Button> m_buttons;
//	//std::vector<Button> m_buttonsMain;
//	//std::vector<Button> m_buttonMulti;
//	//float m_mainButHeigh;
//	//float m_multiButHeight;
//	float m_butHeight;
//	sf::Vector2f m_startButPos;
//	bool m_isMain;
//	//void setButtons(const sf::Vector2u& windowSize);
//	void setButtons(const sf::Vector2u& windowSize, const std::vector<int>& vec);
//};