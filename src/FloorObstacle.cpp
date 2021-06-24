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
    MovingObj(world, startPos, size, bodyType, floorObs, mapEnum),  m_currTimer(startTimer.x), m_currIndex(0),
    m_active(false), m_timer(startTimer.y), m_size(size), m_currentSize(size), m_scaler(size.y / 2)
{
    m_activeTimer = startTimer.x;
    sf::Sprite temp(Resources::getResourceRef().getTexture(mapEnum, floorObs));
    temp.setTextureRect(sf::IntRect(0, 0, FLOOR_OBS_WIDTH, FLOOR_OBS_HEIGHT));
    temp.setScale(size.x / temp.getGlobalBounds().width, (size.y *3.5) / temp.getGlobalBounds().height);
    temp.setOrigin(temp.getTextureRect().width / 2.f, temp.getTextureRect().height / 2.f);
    temp.setPosition(startPos.x, startPos.y-size.y*5/4);
    m_sprite = temp;
    m_sprite.setColor(sf::Color(255, 255, 255, 140));
   // m_sprite.setColor(sf::Color::Magenta);
    /*b2PolygonShape kinematic;
    kinematic.SetAsBox((size.x / SCALE) / 2, (size.y / SCALE) / 2);

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &kinematic;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.3f;
    fixtureDef.filter.categoryBits = fallingBlockBits;

    m_body->CreateFixture(&fixtureDef);*/
    b2PolygonShape shape(std::move(createPolygonShape({ (size.x / SCALE) / 4, (size.y / SCALE) / 2 })));
    createFixtureDef(shape, 1.0f, 0.3, floorObsBit, true, playerBits);
    m_body->SetFixedRotation(true);
    m_body->SetUserData(this);
}

//updates player velocity according to which key is pressed
//applies impulse to jump
void FloorObstacle::updatePhysics(float dt)
{
    //static float timer = m_startingTime;
    //static float ind = 0;
    //static float scaler = m_size.y / 2;

    float absScaler, scalerSign;
    m_currTimer -= dt;
    if (m_currTimer <= 0 && !m_active) {//waiting to go up
        m_active = true;
        m_currTimer = 0;
        m_sprite.setColor(sf::Color(255, 255, 255,255));
    }
    else if (m_currTimer <= 0 && m_active && m_currIndex < FLOOR_OBS_LEN)//going up or down
    {
        m_body->DestroyFixture(m_body->GetFixtureList());
        auto test = m_body->GetFixtureList();
        absScaler = (m_scaler != 0) ? abs(m_scaler) : 0;
        b2PolygonShape shape(std::move(createPolygonShape({ (m_size.x / SCALE) / 4, ((m_size.y+absScaler) / SCALE) / 2 })));
        createFixtureDef(shape, 1.0f, 0.3, floorObsBit,true, playerBits);
        scalerSign = (m_scaler != 0) ? m_scaler / absScaler : -1;
        m_body->SetTransform({ m_body->GetPosition().x, 
                               m_body->GetPosition().y - (((m_size.y / 4)*scalerSign)/SCALE)}, 0);
        m_currIndex++;
        m_col = m_currIndex -1;
        m_sprite.setTextureRect(sf::IntRect(FLOOR_OBS_WIDTH*m_col, FLOOR_OBS_HEIGHT*m_row, FLOOR_OBS_WIDTH, FLOOR_OBS_HEIGHT));
        if (m_currIndex == FLOOR_OBS_LEN && m_scaler > 0) {
            m_currIndex = 0;
            m_scaler *= -1;
            m_row=1;
            m_col = 0;
        }
        m_scaler += m_size.y/2;
        m_currTimer = m_timer;
    }
    else if(m_currIndex == FLOOR_OBS_LEN){
        m_sprite.setColor(sf::Color(255, 255, 255, 140));
        m_col = 0;
        m_sprite.setTextureRect(sf::IntRect(0, 0, FLOOR_OBS_WIDTH, FLOOR_OBS_HEIGHT));
        m_row = 0;
        m_active = false;
        m_currTimer = m_activeTimer;
        m_currIndex = 0;
        m_scaler = m_size.y / 2;
    }
}

void FloorObstacle::move()
{
    //auto position = m_body->GetPosition();
    //auto rotation = m_body->GetAngle();
    //m_sprite.setPosition(position.x * SCALE, position.y * SCALE);
    //m_sprite.setRotation(rotation);
}

void FloorObstacle::draw(sf::RenderWindow& window)
{
    window.draw(m_sprite);
}

void FloorObstacle::setInfo(MovingObjInfo info)
{
    //setPos(info.m_loc);
    //m_timer = info.m_timer;
    //if (m_timer > 0) {
    //    m_body->SetAwake(false);
    //    m_falling = false;
    //}
    //m_body->SetLinearVelocity(info.m_vel);
}

void FloorObstacle::reset()
{
    //m_falling = false;
    //m_body->SetTransform({m_strtPos.x, m_strtPos.y}, 0);
    //m_body->SetAwake(false);
    //m_timer = m_startingTime;
    //m_col = 0;
    //m_activeAnim = false;
    //m_sprite.setTextureRect(sf::IntRect(0, 0, FALLING_WIDTH, FALLING_HEIGHT));
    //setReset(false);
}

void FloorObstacle::updateAnim(float deltaTime) {
   /*     m_sprite.setTextureRect(Animation::getAnimRef().updateAnim(m_row, m_col,
            deltaTime, m_totalTime, floorObs, up, m_startingTime));*/
    //    std::cout << "m:collll: " << m_col << "\n";
}

bool FloorObstacle::getActive() const {
    return m_active;
}