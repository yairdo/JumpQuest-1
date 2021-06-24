#include "Rope.h"
#include "box2d/box2d.h"
#include "Macros.h"
#include <SFML/Graphics.hpp>
#include <Factory.h>
bool Rope::m_registerit = Factory<StaticObj>::registerit("Rope",
    [](b2World& world,int map,std::vector<sf::Vector2f> vec)-> std::unique_ptr<StaticObj>
    { return std::make_unique<Rope>(world, vec[0], vec[1], b2_staticBody,map); });
//-----------------------------------------------------------------------------
Rope::Rope(b2World& world, const sf::Vector2f& pos, const sf::Vector2f& size,
    int bodyType,int mapEnum) :
    StaticObj(world, pos, {0.8f*ROPE_WIDTH,size.y}, bodyType, ROPE_WIDTH,ROPE_HEIGHT, rope, mapEnum)
{
    b2PolygonShape groundBox(std::move(createPolygonShape({ (10 / SCALE) / 2, (size.y / SCALE) / 2 })));
    createFixtureDef(groundBox, 0.f, 0.f, ladderBits,false, playerSensorBits);

    m_body->SetUserData(this);
}
//-----------------------------------------------------------------------------
void Rope::setSprite(float recWidth , float recHeight , const sf::Vector2f& size ,
    const sf::Vector2f& pos ) {
    m_sprite.setScale(size.x, size.y / m_sprite.getGlobalBounds().height);
    m_sprite.setOrigin(m_sprite.getTextureRect().width / 2.f, m_sprite.getTextureRect().height / 2.f);
    m_sprite.setPosition(pos);
}
