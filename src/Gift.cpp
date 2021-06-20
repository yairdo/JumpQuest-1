#include "Gift.h"
#include "box2d/box2d.h"
#include "Macros.h"
#include <iostream>
#include <Factory.h>
//just for debug

bool Gift::m_registerit = Factory<StaticObj>::registerit("Gift",
    [](b2World& world,int map,std::vector<sf::Vector2f> vec) -> std::unique_ptr<StaticObj>
    { return std::make_unique<Gift>(world, vec[0], vec[1], b2_staticBody,map); });

Gift::Gift(b2World& world, const sf::Vector2f& pos, const sf::Vector2f& size, int bodyType,int mapEnum) : 
    StaticObj(world, pos, size, bodyType,gift)
{
    m_sprite.setTextureRect(sf::IntRect(0, 0, GIFT_WIDTH, GIFT_HEIGHT));
    m_sprite.setScale(size.x / m_sprite.getGlobalBounds().width, size.y / m_sprite.getGlobalBounds().height);
    m_sprite.setOrigin(m_sprite.getTextureRect().width / 2.f, m_sprite.getTextureRect().height / 2.f);

    //m_sprite.setColor(sf::Color::Blue);
    /*b2BodyDef groundBodyDef;
    groundBodyDef.position.Set(pos.x / SCALE, pos.y / SCALE);
    m_body = world.CreateBody(&groundBodyDef);*/

    //b2PolygonShape groundBox;
    //groundBox.SetAsBox((size.x / SCALE) / 2, (size.y / SCALE) / 2);
    //b2FixtureDef fixture;
    //fixture.shape = &groundBox;
    //fixture.friction = 1.f;
    //fixture.filter.categoryBits = giftBits;
    //fixture.filter.maskBits = /*0xFFFF &*/ ~noneBit;
    //m_body->CreateFixture(&fixture);

    b2PolygonShape groundBox(std::move(createPolygonShape({ (size.x / SCALE) / 2, (size.y / SCALE) / 2 })));
    createFixtureDef(groundBox, 0.f, 1.f, giftBits, false, ~noneBit);

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

bool Gift::collisionCounter(){
    if (!--m_counter) {
        this->setRemoveObj(true);
        return true;
    }
    m_sprite.setTextureRect(Animation::getAnimRef().updateAnim(m_row, m_col,
       1, m_totalTime, gift, up,1));
    return false;
}

void Gift::MsgCollision(){
    collisionCounter();
}

