#pragma once
#include <SFML/Graphics.hpp>
//#include <Macros.h>
#include <Animation.h>
#include <memory>

class b2Body;
class b2World;

class GameObj {
public:
    GameObj(b2World&, const sf::Vector2f&, const sf::Vector2f&, int, int textureNum);
    GameObj() = default;
    virtual ~GameObj() = default;
    virtual void draw(sf::RenderWindow&) = 0;
    virtual void setRemoveObj(bool );
    virtual bool getIsRemoved() const;
    void destroyBody();
    sf::Vector2f getPos() { return m_sprite.getPosition(); }
    void setPos(sf::Vector2f);
    //=====temp polichecker
    /*virtual void handleCol(GameObj*) = 0;
    virtual void handleGiftCol(Gift*) = 0;*/
    //virtual void handleGiftCol(Block*) = 0;

    //trying stuff for anime -yair
    virtual void updateAnim(float deltaTime);

    int getAnimRow() { return m_row; }
    int getAnimCol() { return m_col; }
  //  int m_row=1, m_col;
   // float m_width=130, m_height=160;
    //float m_totalTime=0;
protected:
    int animPos=idle;
    float m_totalTime = 0;
    int m_row = 1, m_col;
    b2Body* m_body;
    sf::Sprite m_sprite;

private:
    bool m_isRemoved;
};