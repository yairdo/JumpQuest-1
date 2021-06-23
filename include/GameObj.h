#pragma once
#include <SFML/Graphics.hpp>
//#include <Macros.h>
#include <Animation.h>
#include <memory>
#include "box2d/box2d.h"
class b2Body;
class b2World;

class GameObj {
public:
    GameObj(b2World&, const sf::Vector2f&, const sf::Vector2f& size, int,
        float recWidth=0.f,float recHeight=0.f, int textureNum = blank, int mapEnum = castle);
    GameObj() = default;
    virtual ~GameObj() = default;
    virtual void draw(sf::RenderWindow&);
    virtual void setRemoveObj(bool );
    virtual bool getIsRemoved() const;
    virtual void setSprite(float recWidth, float recHeight, const sf::Vector2f& size, const sf::Vector2f& pos);
    void destroyBody();
    sf::Vector2f getPos()const  { return m_sprite.getPosition(); }
    void setPos(sf::Vector2f);
    virtual void MsgCollision(){} //this needs to be abstract 
    bool remove();
    //=====temp polichecker
    /*virtual void handleCol(GameObj*) = 0;
    virtual void handleGiftCol(Gift*) = 0;*/
    //virtual void handleGiftCol(Block*) = 0;

    //trying stuff for anime -yair
    virtual void updateAnim(float deltaTime);

    int getAnimRow() { return m_row; }
    int getAnimCol() { return m_col; }

    bool getCollision() const { return m_collision; }
    void setCollision(bool collision) { m_collision = collision; }
    //virtual void handleCollision();
  //  int m_row=1, m_col;
   // float m_width=130, m_height=160;
    //float m_totalTime=0;
protected:
    bool m_collision;
    int animPos=idle;
    float m_totalTime = 0;
    int m_row, m_col;
    b2Body* m_body;
    sf::Sprite m_sprite;
    b2PolygonShape createPolygonShape(const sf::Vector2f& );
    b2Fixture* createFixtureDef( b2PolygonShape& shape, float density=0.f,
        float friction=0.2f, uint16 categoryBits=1, bool isSensor=false, uint16 maskBits=0xFFFF);

private:
    bool m_isRemoved;
};