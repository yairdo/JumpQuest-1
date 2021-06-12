#include "Block.h"
#include "box2d/box2d.h"
#include "Macros.h"
#include <SFML/Graphics.hpp>

Block::Block(b2World& world, const sf::Vector2f& pos, const sf::Vector2f& size, int bodyType): 
    StaticObj(world, pos, size, bodyType)
{
    b2PolygonShape groundBox;
    groundBox.SetAsBox((size.x/SCALE)/2, (size.y / SCALE) / 2);
    b2FixtureDef fixture;
    fixture.shape = &groundBox;
    fixture.friction = 1.f;
    fixture.filter.categoryBits = wallBits;

    m_body->CreateFixture(&fixture);

    m_sprite.setColor(sf::Color::White);
    //m_body->SetUserData(this);
}

void Block::draw(sf::RenderWindow& window)
{
    window.draw(m_sprite);
}
