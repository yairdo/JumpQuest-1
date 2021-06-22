#include "Rope.h"
#include "box2d/box2d.h"
#include "Macros.h"
#include <SFML/Graphics.hpp>
#include <Factory.h>
bool Rope::m_registerit = Factory<StaticObj>::registerit("Rope",
    [](b2World& world,int map,std::vector<sf::Vector2f> vec)-> std::unique_ptr<StaticObj>
    { return std::make_unique<Rope>(world, vec[0], vec[1], b2_staticBody,map); });

Rope::Rope(b2World& world, const sf::Vector2f& pos, const sf::Vector2f& size,
    int bodyType,int mapEnum) :
    StaticObj(world, pos, {0.8f*ROPE_WIDTH,size.y}, bodyType, ROPE_WIDTH,ROPE_HEIGHT, rope, mapEnum)
{
    //m_sprite=std::move(sf::Sprite(Resources::getResourceRef().getTexture(mapEnum, rope)));
    //setSprite(0.f, 0.f, { 0.8,size.y }, pos);
    //m_sprite.setScale(size.x / m_sprite.getGlobalBounds().width, size.y / m_sprite.getGlobalBounds().height);
    //setSprite(0, 0, { 0.8,size.y }, pos);
    /*m_sprite.setScale(0.8, size.y / m_sprite.getGlobalBounds().height);
    m_sprite.setOrigin(m_sprite.getTextureRect().width / 2.f, m_sprite.getTextureRect().height / 2.f);*/

    /*b2PolygonShape groundBox;
    groundBox.SetAsBox((10 / SCALE)/2, (size.y / SCALE) / 2);
    b2FixtureDef fixture;
    fixture.shape = &groundBox;
    fixture.friction = 0.f;
    fixture.filter.categoryBits = ladderBits;
    fixture.filter.maskBits = playerSensorBits;*/

    //m_body->CreateFixture(&fixture);

    b2PolygonShape groundBox(std::move(createPolygonShape({ (10 / SCALE) / 2, (size.y / SCALE) / 2 })));
    createFixtureDef(groundBox, 0.f, 0.f, ladderBits,false, playerSensorBits);

    m_body->SetUserData(this);
   // m_sprite.setColor(sf::Color::Cyan);
}

void Rope::draw(sf::RenderWindow& window)
{
    window.draw(m_sprite);
}
void Rope::setSprite(float recWidth , float recHeight , const sf::Vector2f& size ,
    const sf::Vector2f& pos ) {
    m_sprite.setScale(size.x, size.y / m_sprite.getGlobalBounds().height);
    m_sprite.setOrigin(m_sprite.getTextureRect().width / 2.f, m_sprite.getTextureRect().height / 2.f);
    m_sprite.setPosition(pos);
}
