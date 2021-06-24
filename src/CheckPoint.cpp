#include "CheckPoint.h"
#include "box2d/box2d.h"
#include "Macros.h"
#include <SFML/Graphics.hpp>
#include <Factory.h>
//-----------------------------------------------------------------------------
//for factory
bool CheckPoint::m_registerit = Factory<StaticObj>::registerit("CheckPoint",
    [](b2World& world,int map, std::vector<sf::Vector2f> vec)-> std::unique_ptr<StaticObj>
    { return std::make_unique<CheckPoint>(world, vec[0], vec[1],vec[2], b2_staticBody, map); });
//-----------------------------------------------------------------------------
//CheckPoint c-tor
CheckPoint::CheckPoint(b2World& world, const sf::Vector2f& pos, const sf::Vector2f& size,
    const sf::Vector2f& winner, int bodyType,int mapEnum) :
    StaticObj(world, pos, size,bodyType, CHECKPOINT_WIDTH, CHECKPOINT_HEIGHT,checkPoint),
    m_activate(false), m_win(winner.x)
{
    if (winner.x) {//marks the checkpoint as the wining checkpoint
        sf::Sprite temp(Resources::getResourceRef().getTexture(mapEnum, finishLine));
        temp.setTextureRect(sf::IntRect(0,0,600,300));
        temp.setScale(size.x*3 / temp.getGlobalBounds().width, size.y*2 / temp.getGlobalBounds().height);
        temp.setOrigin(temp.getTextureRect().width / 2.f, temp.getTextureRect().height/1.5f);
        temp.setPosition(pos.x-size.x/2,pos.y);
        m_sprite = temp;
    }
    b2PolygonShape groundBox(std::move(createPolygonShape({ (size.x / SCALE) / 2, (size.y / SCALE) / 2 })));
    createFixtureDef(groundBox, 0.f, 0.f, checkPointBits, false, playerSensorBits);

    m_body->SetUserData(this);
    m_row = 1;
}
//-----------------------------------------------------------------------------
void CheckPoint::setColor(sf::Color color){
    m_sprite.setColor(color);
}
//-----------------------------------------------------------------------------
//when a player collides with the checkpoint 
void CheckPoint::activate(){
    Resources::getResourceRef().playSound(checkPointSound);
    m_row = 0;
    m_activate = true;
}
//-----------------------------------------------------------------------------
bool CheckPoint::getActive() const{
    return m_activate;
}
//-----------------------------------------------------------------------------
bool CheckPoint::getWin() const{
    return m_win;
}
//-----------------------------------------------------------------------------
//updates the animation of the checkpoint
void CheckPoint::updateAnim(float deltaTime) {
    if(!m_win)
     m_sprite.setTextureRect(Animation::getAnimRef().updateAnim(m_row, m_col,
            deltaTime, m_totalTime, checkPoint, up));
} 
