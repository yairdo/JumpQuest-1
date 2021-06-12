#include "Rope.h"
#include "box2d/box2d.h"
#include "Macros.h"
#include <SFML/Graphics.hpp>

Rope::Rope(b2World& world, const sf::Vector2f& pos, const sf::Vector2f& size, int bodyType) :
    StaticObj(world, pos, size, bodyType)
{
    b2PolygonShape groundBox;
    groundBox.SetAsBox((1 / SCALE)/2, (size.y / SCALE) / 2);
    b2FixtureDef fixture;
    fixture.shape = &groundBox;
    fixture.friction = 0.f;
    fixture.filter.categoryBits = ladderBits;
    fixture.filter.maskBits = playerSensorBits;

    m_body->CreateFixture(&fixture);
    m_body->SetUserData(this);
    m_sprite.setColor(sf::Color::Cyan);
}

void Rope::draw(sf::RenderWindow& window)
{
    window.draw(m_sprite);
}
