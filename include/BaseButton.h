#include <SFML/Graphics.hpp>
#include <string>
#include <StateManager.h>
#include <memory>


class BaseButton {
public:
	BaseButton(int index, const sf::Vector2f& pos, float width, float height, bool isExit=false);
	//BaseButton(int index, const sf::Vector2f& pos, float height);
	void setTransp(const sf::Vector2f& loc);
	void draw(sf::RenderWindow&)const;
	bool checkCollision(const sf::Vector2f& loc) const { return m_title.getGlobalBounds().contains(loc); }
	virtual std::unique_ptr<State> ButtonState(StateManager&, sf::RenderWindow&, bool, std::shared_ptr<NetworkObject>&) const { return nullptr; };
	bool getIsExit()const { return m_isExit; }
	
private:
	sf::Sprite m_title;
	bool m_isExit;
	//int m_enumIndex;
};