#include "Rope.h"
#include "box2d/box2d.h"
#include "Macros.h"
#include <SFML/Graphics.hpp>
#include <Factory.h>
bool Rope::m_registerit = Factory<StaticObj>::registerit("Rope",
    [](b2World& world,std::vector<sf::Vector2f> vec)-> std::unique_ptr<StaticObj>
    { return std::make_unique<Rope>(world, vec[0], vec[1], b2_staticBody); });

Rope::Rope(b2World& world, const sf::Vector2f& pos, const sf::Vector2f& size, int bodyType) :
    StaticObj(world, pos, size, bodyType,rope)
{

    //m_sprite.setScale(size.x / m_sprite.getGlobalBounds().width, size.y / m_sprite.getGlobalBounds().height);
    m_sprite.setScale(0.8, size.y / m_sprite.getGlobalBounds().height);
    m_sprite.setOrigin(m_sprite.getTextureRect().width / 2.f, m_sprite.getTextureRect().height / 2.f);

    b2PolygonShape groundBox;
    groundBox.SetAsBox((1 / SCALE)/2, (size.y / SCALE) / 2);
    b2FixtureDef fixture;
    fixture.shape = &groundBox;
    fixture.friction = 0.f;
    fixture.filter.categoryBits = ladderBits;
    fixture.filter.maskBits = playerSensorBits;

    m_body->CreateFixture(&fixture);
    m_body->SetUserData(this);
   // m_sprite.setColor(sf::Color::Cyan);
}

void Rope::draw(sf::RenderWindow& window)
{
    window.draw(m_sprite);
}
