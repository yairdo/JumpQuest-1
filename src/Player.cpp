#include "Player.h"
#include "box2d/box2d.h"
#include "Macros.h"
#include <SFML/Graphics.hpp>

Player::Player(b2World& world, const sf::Vector2f& pos, const sf::Vector2f& size, int bodyType):
    MovingObj(world, pos, size, b2_dynamicBody,player), m_numFootContact(0)
{
    m_sprite.setColor(sf::Color::Green);

    m_body->SetFixedRotation(true);
    b2PolygonShape dynamicBox;
    dynamicBox.SetAsBox(size.x/(2.f*SCALE), size.y / (2.f * SCALE));
    
    //set player shape
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &dynamicBox;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.3f;
    fixtureDef.filter.categoryBits = playerBits;

    m_body->CreateFixture(&fixtureDef);
    //--sides fixture
    dynamicBox.SetAsBox(size.x / (2.f * SCALE)+0.01f, size.y / (2.f * SCALE)-0.02f);
    fixtureDef.friction = 0;
    fixtureDef.filter.maskBits = wallBits | movingBlockBits | boundryBits | fallingBlockBits | giftBits;

    m_body->CreateFixture(&fixtureDef);
    //set up sensor
    dynamicBox.SetAsBox((1 / SCALE)/2, size.y / (2.f * SCALE));
    //fixtureDef.friction = 0;
    fixtureDef.isSensor = true;
    fixtureDef.filter.categoryBits = playerSensorBits;
    fixtureDef.filter.maskBits = ladderBits;
    m_body->CreateFixture(&fixtureDef);
    m_body->SetUserData(this);
    // add foot sensor fixture
    dynamicBox.SetAsBox(size.x/(SCALE*2), 1 / (SCALE * 2), b2Vec2(0, size.y / (2.f * SCALE)), 0);
    fixtureDef.isSensor = true;
    fixtureDef.filter.categoryBits = footBits;
    fixtureDef.filter.maskBits = 0xFFFF;
    b2Fixture* footSensorFixture = m_body->CreateFixture(&fixtureDef);
    footSensorFixture->SetUserData((void*)FOOT);
}

//updates player velocity according to which key is pressed
//applies impulse to jump
void Player::updatePhysics(float dt)
{
    if(m_onRope)
        m_body->SetLinearVelocity({ 0.f, 0.f });

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
        jump();
    }
    else if ( sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
        m_direction = up;
        if(m_onRope)
            m_body->SetLinearVelocity({ 0.f, -1.5f });
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
        m_direction = right;
        if (m_onRope)
            return;
        else
            m_body->SetLinearVelocity({ 1.5f, m_body->GetLinearVelocity().y });
    }
    else if (m_onRope && (sf::Keyboard::isKeyPressed(sf::Keyboard::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Down))) {
        m_direction = down;
        m_body->SetLinearVelocity({ 0.f, 1.5f });
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
        m_direction = left;
        if (m_onRope)
            return;
        else
            m_body->SetLinearVelocity({ -1.5f, m_body->GetLinearVelocity().y });
    }
}

void Player::jump() {
    float impulse = -m_body->GetMass() * 150;
    if (m_onRope) {
        //m_body->SetLinearVelocity({ 0.f, 0.f });
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
            m_body->ApplyForce(b2Vec2(impulse, impulse/2), m_body->GetWorldCenter(), true);
            setOnRope(false);
        }
        else if(sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right)){
            m_body->ApplyForce(b2Vec2(-impulse, impulse/2), m_body->GetWorldCenter(), true);
            setOnRope(false);
        }
        return;
    }
    if (m_numFootContact)
        m_body->ApplyForce(b2Vec2(0, impulse), m_body->GetWorldCenter(), true);
}

void Player::move()
{
    auto position = m_body->GetPosition();
    m_sprite.setPosition(position.x * SCALE, position.y * SCALE);
    
}

void Player::draw(sf::RenderWindow& window)
{
    window.draw(m_sprite);
}

void Player::setOnRope(bool val){
    m_onRope = val;
    if (m_onRope) {
        m_body->SetLinearVelocity({ 0.f, 0.f });
        m_body->SetGravityScale(0);
        return;
    }
    m_body->SetGravityScale(1);
}

sf::Vector2f Player::getPos() {
   return m_sprite.getPosition();
}

int Player::getDirection()
{
    return m_direction;
}

void Player::footContact(int val) {
    m_numFootContact += val;
}
