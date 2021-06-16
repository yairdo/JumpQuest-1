#include "Gift.h"
#include "box2d/box2d.h"
#include "Macros.h"
#include <iostream>
#include <Factory.h>
//just for debug

bool Gift::m_registerit = Factory<StaticObj>::registerit("Gift",
    [](b2World& world,std::vector<sf::Vector2f> vec) -> std::unique_ptr<StaticObj>
    { return std::make_unique<Gift>(world, vec[0], vec[1], b2_staticBody); });

Gift::Gift(b2World& world, const sf::Vector2f& pos, const sf::Vector2f& size, int bodyType) : 
    StaticObj(world, pos, size, bodyType)
{
    m_sprite.setScale(size.x / m_sprite.getGlobalBounds().width, size.y / m_sprite.getGlobalBounds().height);
    m_sprite.setOrigin(m_sprite.getTextureRect().width / 2.f, m_sprite.getTextureRect().height / 2.f);

    m_sprite.setColor(sf::Color::Blue);
    /*b2BodyDef groundBodyDef;
    groundBodyDef.position.Set(pos.x / SCALE, pos.y / SCALE);
    m_body = world.CreateBody(&groundBodyDef);*/

    b2PolygonShape groundBox;
    groundBox.SetAsBox((size.x / SCALE) / 2, (size.y / SCALE) / 2);
    b2FixtureDef fixture;
    fixture.shape = &groundBox;
    fixture.friction = 1.f;
    fixture.filter.categoryBits = giftBits;

    m_body->CreateFixture(&fixture);

    /*m_testRect.setFillColor(sf::Color::Blue);
    m_testRect.setOrigin(size / 2.f);
    m_testRect.setPosition({ m_body->GetPosition().x * SCALE, m_body->GetPosition().y * SCALE });*/
    m_body->SetUserData((GameObj*)this);
}

Gift::~Gift()
{
}

void Gift::draw(sf::RenderWindow& window)
{
    window.draw(m_sprite);
}

void Gift::collisionCounter(){
    if (!--m_counter)
        this->setRemoveObj(true);
}

void Gift::MsgCollision(){
    collisionCounter();
}