#include "FallingBlock.h"
#include "box2d/box2d.h"
#include "Macros.h"
#include <SFML/Graphics.hpp>
#include <Factory.h>
bool FallingObj::m_registerit = Factory<MovingObj>::registerit("FallingBlock",
    [](b2World& world,int map,std::vector<sf::Vector2f> vec)-> std::unique_ptr<MovingObj>
    { return std::make_unique<FallingObj>(world, vec[0], vec[1], b2_dynamicBody, map); });

FallingObj::FallingObj(b2World& world, const sf::Vector2f& startPos, const sf::Vector2f& size,
    int bodyType,int mapEnum) :
    m_strtPos(startPos / SCALE), MovingObj(world, startPos, size, bodyType, fallingBlock, mapEnum), m_activeAnim(false)
{

    m_sprite.setTextureRect(sf::IntRect(0, 0, FALLING_WIDTH, FALLING_HEIGHT));
    m_sprite.setScale(size.x / m_sprite.getGlobalBounds().width, size.y / m_sprite.getGlobalBounds().height);
    m_sprite.setOrigin(m_sprite.getTextureRect().width / 2.f, m_sprite.getTextureRect().height / 2.f);

   // m_sprite.setColor(sf::Color::Magenta);
    /*b2PolygonShape kinematic;
    kinematic.SetAsBox((size.x / SCALE) / 2, (size.y / SCALE) / 2);

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &kinematic;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.3f;
    fixtureDef.filter.categoryBits = fallingBlockBits;

    m_body->CreateFixture(&fixtureDef);*/
    b2PolygonShape kinmatic(std::move(createPolygonShape({ (size.x / SCALE) / 2, (size.y / SCALE) / 2 })));
    createFixtureDef(kinmatic, 1.0f, 0.3, fallingBlockBits);
    m_body->SetFixedRotation(true);
    m_body->SetUserData(this);
    m_body->SetAwake(false);
}

//updates player velocity according to which key is pressed
//applies impulse to jump
void FallingObj::updatePhysics(float dt)
{
    if (!m_falling && m_timer <= 0)//going up or down
    {
        m_body->ApplyForceToCenter({ 0, dt*5.f }, true);
        m_falling = true;
        return;
    }
    if (getReset() || (m_timer <= 0 && !m_body->IsAwake()))
        reset();
    m_timer -= dt;
}

void FallingObj::move()
{
    auto position = m_body->GetPosition();
    auto rotation = m_body->GetAngle();
    m_sprite.setPosition(position.x * SCALE, position.y * SCALE);
    m_sprite.setRotation(rotation);
}

void FallingObj::draw(sf::RenderWindow& window)
{
    window.draw(m_sprite);
}

void FallingObj::setInfo(MovingObjInfo info)
{
    setPos(info.m_loc);
    m_timer = info.m_timer;
    if (m_timer > 0) {
        m_body->SetAwake(false);
        m_falling = false;
    }
    m_body->SetLinearVelocity(info.m_vel);
}

void FallingObj::reset()
{
    m_falling = false;
    m_body->SetTransform({m_strtPos.x, m_strtPos.y}, 0);
    m_body->SetAwake(false);
    m_timer = 3;
    m_col = 0;
    //m_activeAnim = false;
    m_sprite.setTextureRect(sf::IntRect(0, 0, FALLING_WIDTH, FALLING_HEIGHT));
    setReset(false);
}

void FallingObj::updateAnim(float deltaTime) {
    //b2Vec2 vec{ 0, 0 };
    //double eps = 0.00001;
    //if (m_falling && (m_col < FALLING_LEN-1))
    //    ++m_col;
    //if (m_falling && (m_body->GetLinearVelocity().x <eps && m_body->GetLinearVelocity().y <eps))
    //    m_sprite.setTextureRect(Animation::getAnimRef().updateAnim(0, m_col,
    //        deltaTime, m_totalTime, fallingBlock, left,FALLING_SWITCH_TIME));
    // 
    //if (m_falling && (m_col < FALLING_LEN - 1))
    //    ++m_col;
    //if(m_activeAnim)
    //    m_sprite.setTextureRect(Animation::getAnimRef().updateAnim(0, m_col,
    //        deltaTime, m_totalTime, fallingBlock, left, FALLING_SWITCH_TIME));
}

void FallingObj::setActiveAnim() {
    m_activeAnim = true;
}