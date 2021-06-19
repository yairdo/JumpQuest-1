#pragma once
#include <vector>
#include <SFML/Graphics.hpp>
#include "State.h"
#include "Button.h"

class StateManager;

class MenuState: public State{
public:

	MenuState(StateManager& manager,sf::RenderWindow& window, bool replace
			 ,std::shared_ptr<NetworkObject> net,int title, int background);
	MenuState(StateManager& manager, sf::RenderWindow& window, bool replace);
	virtual ~MenuState()=default;
	virtual void draw() ;
	virtual void update();
	virtual void setTransp(const sf::Vector2f&) const;
	virtual void updateNextState(const sf::Vector2f& loc);
	virtual void pause() {};
	virtual void resume() {};
	
protected:
	void setTitle(sf::Sprite&& title);
	template <class T>
	void addButton(int index,const sf::Vector2f& loc, float width, float height, bool ext=false);
	std::vector<std::unique_ptr<BaseButton>> m_buttons;
	float getTitleHeight() const;
	sf::Vector2f m_middle;
	float getTitlePosY() const;
private:

	std::unique_ptr<sf::Sprite> m_background;
	std::unique_ptr<sf::Sprite> m_title;
	void setTitle(int);
};
template <class T>
void MenuState::addButton(int index,const sf::Vector2f& loc, float width, float height, bool ext){
	m_buttons.emplace_back(std::make_unique<Button<T>>(index,loc,width,height,ext));
}