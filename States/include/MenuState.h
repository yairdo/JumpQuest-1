#pragma once
#include <vector>
#include <SFML/Graphics.hpp>
#include "State.h"
#include "Button.h"

class StateManager;

class MenuState: public State{
public:
	//-----------c-tor------------
	MenuState(StateManager& manager,sf::RenderWindow& window, bool replace
			 ,std::shared_ptr<NetworkObject> net,int title, int background);
	MenuState(StateManager& manager, sf::RenderWindow& window, bool replace);

	//-----------d-tor------------
	virtual ~MenuState()=default;

	//----------Functions---------
	virtual void draw() override;
	virtual void update() override;
	virtual void pause() override {};
	virtual void resume() override {};
	
protected:
	//-----------Members----------

	std::vector<std::unique_ptr<BaseButton>> m_buttons;
	sf::Vector2f m_middle;

	//----------Functions---------

	virtual void updateNextState(const sf::Vector2f& loc);
	template <class T>
	void addButton(int index,const sf::Vector2f& loc, float width,
		float height, bool ext=false);
	float getTitlePosY() const;
private:
	//-----------Members----------
	std::unique_ptr<sf::Sprite> m_background;
	std::unique_ptr<sf::Sprite> m_title;

	//----------Functions---------
	void setTitle(int);
	virtual void setTransp(const sf::Vector2f&) const;
};
template <class T>
void MenuState::addButton(int index,const sf::Vector2f& loc, float width, float height, bool ext){
	m_buttons.emplace_back(std::make_unique<Button<T>>(index,loc,width,height,ext));
}