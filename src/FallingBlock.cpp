#include "FallingBlock.h"
#include "box2d/box2d.h"
#include "Macros.h"
#include <SFML/Graphics.hpp>
#include <Factory.h>
bool FallingBlock::m_registerit = Factory<MovingObj>::registerit("FallingBlock",
    [](b2World& world,std::vector<sf::Vector2f> vec)-> std::unique_ptr<MovingObj>
    { return std::make_unique<FallingBlock>(world, vec[0], vec[1], b2_dynamicBody); });

FallingBlock::FallingBlock(b2World& world, const sf::Vector2f& startPos, const sf::Vector2f& size, int bodyType) :
    m_strtPos(startPos / SCALE), MovingObj(world, startPos, size, bodyType, fallingBlock), m_activeAnim(false)
{
    m_sprite.setTextureRect(sf::IntRect(0, 0, FALLING_WIDTH, FALLING_HEIGHT));
    m_sprite.setScale(size.x / m_sprite.getGlobalBounds().width, size.y / m_sprite.getGlobalBounds().height);
    m_sprite.setOrigin(m_sprite.getTextureRect().width / 2.f, m_sprite.getTextureRect().height / 2.f);

   // m_sprite.setColor(sf::Color::Magenta);
    b2PolygonShape kinematic;
    kinematic.SetAsBox((size.x / SCALE) / 2, (size.y / SCALE) / 2);

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &kinematic;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.3f;
    fixtureDef.filter.categoryBits = fallingBlockBits;

    m_body->CreateFixture(&fixtureDef);
    m_body->SetUserData(this);
    m_body->SetAwake(false);
}

//updates player velocity according to which key is pressed
//applies impulse to jump
void FallingBlock::updatePhysics(float dt)
{
    if (!m_falling && m_timer <= 0)//going up or down
    {
        m_body->ApplyForceToCenter({ 0, dt*5.f }, true);
        m_falling = true;
        return;
    }
    if (m_timer <= 0 && !m_body->IsAwake())
        reset();
    m_timer -= dt;
}

void FallingBlock::move()
{
    auto position = m_body->GetPosition();
    auto rotation = m_body->GetAngle();
    m_sprite.setPosition(position.x * SCALE, position.y * SCALE);
    m_sprite.setRotation(rotation);
}

void FallingBlock::draw(sf::RenderWindow& window)
{
    window.draw(m_sprite);
}

void FallingBlock::setInfo(MovingObjInfo info)
{
    setPos(info.m_loc);
    m_timer = info.m_timer;
    if (m_timer > 0) {
        m_body->SetAwake(false);
        m_falling = false;
    }
    m_body->SetLinearVelocity(info.m_vel);
}

void FallingBlock::reset()
{
    m_falling = false;
    m_body->SetTransform({m_strtPos.x, m_strtPos.y}, 0);
    m_body->SetAwake(false);
    m_timer = 3;
}

sf::Vector2f FallingBlock::getPos() {
    return m_sprite.getPosition();
}

void FallingBlock::updateAnim(float deltaTime) {
    if (m_activeAnim) 
       m_sprite.setTextureRect(Animation::getAnimRef().updateAnim(0, m_col,
         deltaTime, m_totalTime, fallingBlock, left));
}

void FallingBlock::setActiveAnim(bool state) {
    m_activeAnim = state;
}