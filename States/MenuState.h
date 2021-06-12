#pragma once
#include <vector>
#include <SFML/Graphics.hpp>
#include "State.h"
#include "Button.h"

class StateManager;

class MenuState: public State{
public:

	MenuState(StateManager& manager,sf::RenderWindow& window, bool replace
			 ,std::shared_ptr<NetworkObject>& net, int background);
	virtual ~MenuState()=default;
	virtual void draw() ;
	virtual void update();
	//virtual void setMenuState() const=0;
	virtual void setTransp(const sf::Vector2f&);
	virtual void updateNextState(const sf::Vector2f& loc);
	
	//????? maybe singelton
	//int returnState();
	//void setState(int);

	virtual void pause() {};
	virtual void resume() {};
	//virtual void draw() = 0;
protected:
	//const Menuu* m_menu;
	//void setTitle(const sf::Vector2f& titleScale, const sf::Vector2f& loc, int title);
	void setTitle(sf::Sprite&& title);
	template <class T>
	void addButton(int index,const sf::Vector2f& loc, float width, float height, bool ext=false);
	//setButton();
	std::unique_ptr<sf::Sprite> m_title;
	std::vector<std::unique_ptr<BaseButton>> m_buttons;
private:
	std::unique_ptr<sf::Sprite> m_background;
	
	
};
template <class T>
void MenuState::addButton(int index,const sf::Vector2f& loc, float width, float height, bool ext){
	m_buttons.emplace_back(std::make_unique<Button<T>>(index,loc,width,height,ext));
}