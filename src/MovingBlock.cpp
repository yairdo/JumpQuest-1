#include "MovingBlock.h"
#include "box2d/box2d.h"
#include "Macros.h"
#include <SFML/Graphics.hpp>
#include <Factory.h>
//-----------------------------------------------------------------------------
bool MovingBlock::m_registerit = Factory<MovingObj>::registerit("MovingBlock",
    [](b2World& world,int map,std::vector<sf::Vector2f> vec)-> std::unique_ptr<MovingObj>
    { return std::make_unique<MovingBlock>(world, vec[0], vec[1], vec[2], b2_kinematicBody,map); });
//-----------------------------------------------------------------------------
//c-tor
MovingBlock::MovingBlock(b2World& world, const sf::Vector2f& startPos, const sf::Vector2f& endPos,
    const sf::Vector2f& size, int bodyType,int mapEnum) :
    m_strtPos(startPos/SCALE), m_endPos(endPos/SCALE),
    MovingObj(world, startPos, size, bodyType,0, 0, block, mapEnum)
{

    b2PolygonShape shape(std::move(createPolygonShape({ (size.x / SCALE) / 2, ((size.y-2) / SCALE) / 2 })));
    createFixtureDef(shape, 1.f, 0.f, noHandleBit);
    shape.SetAsBox((size.x) / (SCALE*2), 0.5 / SCALE*2, b2Vec2(0, -(size.y-0.5)/ (2.f * SCALE)),0);
    createFixtureDef(shape, 1.f, 1.f, movingBlockBits);

    m_body->SetUserData(this);
}
//-----------------------------------------------------------------------------
//updates the MovingBlock physics - sets the velocity in the direction which 
//we want the moving block to move, when he reaches his destination he moves
//to the opposite direction
void MovingBlock::updatePhysics(float dt)
{
    auto pos = m_body->GetPosition();
    b2Vec2 vel = m_body->GetLinearVelocity();

    if (m_strtPos.y == m_endPos.y)//going up or down
    {
        if(pos.x <= m_strtPos.x)
            m_body->SetLinearVelocity({ MOVING_BLOCK_SPEED, 0 });
        else if (pos.x >= m_endPos.x)
            m_body->SetLinearVelocity({ -MOVING_BLOCK_SPEED, 0 });
        else 
            m_body->SetLinearVelocity(vel);
    }
    else {
        if (pos.y <= m_strtPos.y)
            m_body->SetLinearVelocity({ 0, MOVING_BLOCK_SPEED});
        else if (pos.y >= m_endPos.y)
            m_body->SetLinearVelocity({ 0, -MOVING_BLOCK_SPEED });
        else
            m_body->SetLinearVelocity(vel);
    }
}
//-----------------------------------------------------------------------------
//moves the block according to physics body position
void MovingBlock::move()
{
    auto position = m_body->GetPosition();
    m_sprite.setPosition(position.x * SCALE, position.y * SCALE);
}
//-----------------------------------------------------------------------------
float MovingBlock::getWidth() const {
    return m_sprite.getGlobalBounds().width;
}
