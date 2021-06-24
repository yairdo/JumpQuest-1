#include "FallingBlock.h"
#include "box2d/box2d.h"
#include "Macros.h"
#include <SFML/Graphics.hpp>
#include <Factory.h>

//for factory
bool FallingBlock::m_registerit = Factory<MovingObj>::registerit("FallingBlock",
    [](b2World& world,int map,std::vector<sf::Vector2f> vec)-> std::unique_ptr<MovingObj>
    { return std::make_unique<FallingBlock>(world, vec[0], vec[1], vec[2], b2_dynamicBody, map); });

/*FallingBlock c-tor 
*/
FallingBlock::FallingBlock(b2World& world, const sf::Vector2f& startPos, const sf::Vector2f& size, 
    const sf::Vector2f& startTimer, int bodyType,int mapEnum) :
    m_strtPos(startPos / SCALE),
    MovingObj(world, startPos, size, bodyType, FALLING_WIDTH, FALLING_HEIGHT, fallingBlock, mapEnum),
    m_activeAnim(false), m_startingTime(startTimer.x)
{
    m_activeTimer = startTimer.x;
    b2PolygonShape kinmatic(std::move(createPolygonShape({ (size.x / SCALE) / 2, (size.y / SCALE) / 2 })));
    createFixtureDef(kinmatic, 1.0f, 0.3, fallingBlockBits);
    m_body->SetFixedRotation(true);
    m_body->SetUserData(this);
    m_body->SetAwake(false);
}

//updates player velocity according to which key is pressed
//applies impulse to jump
void FallingBlock::updatePhysics(float dt)
{
    if (!m_falling && m_activeTimer <= 0)//going up or down
    {
        m_body->ApplyForceToCenter({ 0, dt*5.f }, true);
        m_falling = true;
        return;
    }
    if (getReset() || (m_activeTimer <= 0 && !m_body->IsAwake()))
        reset();
    m_activeTimer -= dt;
}

void FallingBlock::move()
{
    auto position = m_body->GetPosition();
    auto rotation = m_body->GetAngle();
    m_sprite.setPosition(position.x * SCALE, position.y * SCALE);
    m_sprite.setRotation(rotation);
}

void FallingBlock::setInfo(const MovingObjInfo& info)
{
    setPos(info.m_loc);
    m_activeTimer = info.m_timer;
    if (m_activeTimer > 0) {
        m_body->SetAwake(false);
        m_falling = false;
    }
    m_body->SetLinearVelocity(info.m_vel);
    if (m_activeAnim && !info.m_active)
        reset();
    m_activeAnim = info.m_active;
}

void FallingBlock::reset()
{
    m_falling = false;
    m_body->SetTransform({m_strtPos.x, m_strtPos.y}, 0);
    m_body->SetAwake(false);
    m_activeTimer = m_startingTime;
    m_col = 0;
    m_activeAnim = false;
    m_sprite.setTextureRect(sf::IntRect(0, 0, FALLING_WIDTH, FALLING_HEIGHT));
    setReset(false);
}

void FallingBlock::updateAnim(float deltaTime) {
    if (m_activeAnim)
        m_sprite.setTextureRect(sf::IntRect(FALLING_WIDTH*(FALLING_LEN-1), 0, FALLING_WIDTH, FALLING_HEIGHT));
}

void FallingBlock::setActiveAnim() {
    m_activeAnim = true;
}
