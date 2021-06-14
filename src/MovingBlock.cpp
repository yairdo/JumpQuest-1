#include "MovingBlock.h"
#include "box2d/box2d.h"
#include "Macros.h"
#include <SFML/Graphics.hpp>
#include <Factory.h>

bool MovingBlock::m_registerit = Factory<MovingObj>::registerit("MovingBlock",
    [](b2World& world,std::vector<sf::Vector2f> vec)-> std::unique_ptr<MovingObj>
    { return std::make_unique<MovingBlock>(world, vec[0], vec[1], vec[2], b2_kinematicBody); });

MovingBlock::MovingBlock(b2World& world, const sf::Vector2f& startPos, const sf::Vector2f& endPos, const sf::Vector2f& size, int bodyType) :
    m_strtPos(startPos/SCALE), m_endPos(endPos/SCALE), MovingObj(world, startPos, size, bodyType)
{
    m_sprite.setScale(size.x / m_sprite.getGlobalBounds().width, size.y / m_sprite.getGlobalBounds().height);
    m_sprite.setOrigin(m_sprite.getTextureRect().width / 2.f, m_sprite.getTextureRect().height / 2.f);

    m_sprite.setColor(sf::Color::Red);
    b2PolygonShape kinematic;
    kinematic.SetAsBox((size.x / SCALE) / 2, (size.y / SCALE) / 2);

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &kinematic;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 1.f;
    fixtureDef.filter.categoryBits = movingBlockBits;

    m_body->CreateFixture(&fixtureDef);
    m_body->SetUserData(this);
}

//updates player velocity according to which key is pressed
//applies impulse to jump
void MovingBlock::updatePhysics(float dt)
{
    auto pos = m_body->GetPosition();
    b2Vec2 vel = m_body->GetLinearVelocity();

    if (m_strtPos.y == m_endPos.y)//going up or down
    {
        if(pos.x <= m_strtPos.x)
            m_body->SetLinearVelocity({ .5f, 0 });
        else if (pos.x >= m_endPos.x)
            m_body->SetLinearVelocity({ -.5f, 0 });
        else 
            m_body->SetLinearVelocity(vel);
    }
    else {
        if (pos.y <= m_strtPos.y)
            m_body->SetLinearVelocity({ 0, .5f});
        else if (pos.y >= m_endPos.y)
            m_body->SetLinearVelocity({ 0, -.5f });
        else
            m_body->SetLinearVelocity(vel);
    }
}

void MovingBlock::move()
{
    auto position = m_body->GetPosition();
    m_sprite.setPosition(position.x * SCALE, position.y * SCALE);
}

void MovingBlock::draw(sf::RenderWindow& window)
{
    window.draw(m_sprite);
}

sf::Vector2f MovingBlock::getPos() {
    return m_sprite.getPosition();
}