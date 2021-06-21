#include "FloorObstacle.h"
#include "box2d/box2d.h"
#include "Macros.h"
#include <SFML/Graphics.hpp>
#include <Factory.h>
bool FloorObstacle::m_registerit = Factory<MovingObj>::registerit("FloorObstacle",
    [](b2World& world,int map,std::vector<sf::Vector2f> vec)-> std::unique_ptr<MovingObj>
    { return std::make_unique<FloorObstacle>(world, vec[0], vec[1], vec[2], b2_staticBody, map); });

FloorObstacle::FloorObstacle(b2World& world, const sf::Vector2f& startPos, const sf::Vector2f& size,
    const sf::Vector2f& startTimer, int bodyType,int mapEnum) :
    m_strtPos(startPos / SCALE), MovingObj(world, startPos, size, bodyType, fallingBlock, mapEnum),  
    m_activeAnim(false), m_startingTime(startTimer.x),m_timer(startTimer.x), m_size(size)
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
    b2PolygonShape shape(std::move(createPolygonShape({ (size.x / SCALE) / 2, (size.y / SCALE) / 2 })));
    createFixtureDef(shape, 1.0f, 0.3, floorObsBit);
    m_body->SetFixedRotation(true);
    m_body->SetUserData(this);
    m_body->SetAwake(false);
}

//updates player velocity according to which key is pressed
//applies impulse to jump
void FloorObstacle::updatePhysics(float dt)
{
    static float timer = m_startingTime;
    static float ind = FLOOR_OBS_LEN;
    static float scaler = 1.5;

    if (timer <= 0 && !m_active) {//
        m_active = true;
        timer = m_timer;
    }
    else if (m_active && ind < FLOOR_OBS_LEN)//going up or down
    {
        m_body->DestroyFixture(m_body->GetFixtureList()[0]);
        b2PolygonShape shape(std::move(createPolygonShape({ (m_size.x / SCALE) / 2, (m_size.y*scaler / SCALE) / 2 })));
        createFixtureDef(shape, 1.0f, 0.3, floorObsBit, true);
        scaler += 0.5;
        ind++;

    }
    if (getReset() || (m_timer <= 0))
        reset();
    m_timer -= dt;
}

void FloorObstacle::move()
{
    auto position = m_body->GetPosition();
    auto rotation = m_body->GetAngle();
    m_sprite.setPosition(position.x * SCALE, position.y * SCALE);
    m_sprite.setRotation(rotation);
}

void FloorObstacle::draw(sf::RenderWindow& window)
{
    window.draw(m_sprite);
}

void FloorObstacle::setInfo(MovingObjInfo info)
{
    setPos(info.m_loc);
    m_timer = info.m_timer;
    if (m_timer > 0) {
        m_body->SetAwake(false);
        m_falling = false;
    }
    m_body->SetLinearVelocity(info.m_vel);
}

void FloorObstacle::reset()
{
    m_falling = false;
    m_body->SetTransform({m_strtPos.x, m_strtPos.y}, 0);
    m_body->SetAwake(false);
    m_timer = m_startingTime;
    m_col = 0;
    m_activeAnim = false;
    m_sprite.setTextureRect(sf::IntRect(0, 0, FALLING_WIDTH, FALLING_HEIGHT));
    setReset(false);
}

void FloorObstacle::updateAnim(float deltaTime) {
    //b2Vec2 vec{ 0, 0 };
    //double eps = 0.00001;
    //if (m_falling && (m_col < FALLING_LEN-1))
    //    ++m_col;
    //if (m_falling && (m_body->GetLinearVelocity().x <eps && m_body->GetLinearVelocity().y <eps))
    //    m_sprite.setTextureRect(Animation::getAnimRef().updateAnim(0, m_col,
    //        deltaTime, m_totalTime, fallingBlock, left,FALLING_SWITCH_TIME));
    // 
    if (m_falling && (m_col < FALLING_LEN - 1))
        ++m_col;
    if(m_activeAnim)
        m_sprite.setTextureRect(Animation::getAnimRef().updateAnim(0, m_col,
            deltaTime, m_totalTime, fallingBlock, left, FALLING_SWITCH_TIME));
}

void FloorObstacle::setActiveAnim() {
    m_activeAnim = true;
}