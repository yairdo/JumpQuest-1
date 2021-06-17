#include "CheckPoint.h"
#include "box2d/box2d.h"
#include "Macros.h"
#include <SFML/Graphics.hpp>
#include <Factory.h>

bool CheckPoint::m_registerit = Factory<StaticObj>::registerit("CheckPoint",
    [](b2World& world, std::vector<sf::Vector2f> vec)-> std::unique_ptr<StaticObj>
    { return std::make_unique<CheckPoint>(world, vec[0], vec[1], b2_staticBody); });

CheckPoint::CheckPoint(b2World& world, const sf::Vector2f& pos, const sf::Vector2f& size, int bodyType) :
    StaticObj(world, pos, size, bodyType/*, checkPoint*/), m_activate(false) {

    m_sprite.setScale(size.x / m_sprite.getGlobalBounds().width, size.y / m_sprite.getGlobalBounds().height);
    m_sprite.setOrigin(m_sprite.getTextureRect().width / 2.f, m_sprite.getTextureRect().height / 2.f);

    b2PolygonShape groundBox;
    groundBox.SetAsBox((size.x / SCALE) / 2, (size.y / SCALE) / 2);
    b2FixtureDef fixture;
    fixture.shape = &groundBox;
    fixture.friction = 0.f;
    fixture.filter.categoryBits = checkPointBits;
    fixture.filter.maskBits = playerSensorBits;

    m_body->CreateFixture(&fixture);
    m_body->SetUserData(this);
    m_sprite.setColor(sf::Color::Yellow);
}

void CheckPoint::draw(sf::RenderWindow& window)
{
    window.draw(m_sprite);
}

void CheckPoint::setColor(sf::Color color){
    m_sprite.setColor(color);
}

void CheckPoint::activate(){
    m_sprite.setColor(sf::Color::Magenta);
    m_activate = true;
}

bool CheckPoint::getActive() const
{
    return m_activate;
}
