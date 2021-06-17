#include "Player.h"
#include "box2d/box2d.h"
#include "Macros.h"
#include <SFML/Graphics.hpp>

Player::Player(b2World& world, const sf::Vector2f& pos, const sf::Vector2f& size, int bodyType):
    MovingObj(world, pos, size, b2_dynamicBody,player), m_numFootContact(0), m_checkPoint(pos)
{

 //   m_sprite.setOrigin(m_sprite.getTextureRect().width / 2.f, m_sprite.getTextureRect().height / 2.f); 
  //  m_sprite.setColor(sf::Color::Green);
    m_sprite.setTextureRect(sf::IntRect(0, 0, PLAYER_WIDTH, PLAYER_HEIGHT));
    m_sprite.setScale(size.x / m_sprite.getGlobalBounds().width, size.y / m_sprite.getGlobalBounds().height);
    m_sprite.setOrigin(m_sprite.getTextureRect().width / 2.f, m_sprite.getTextureRect().height / 2.f);
    m_body->SetFixedRotation(true);
    b2PolygonShape dynamicBox;
    dynamicBox.SetAsBox(size.x/(4.f*SCALE), size.y / (2.f * SCALE));
    
    //set player shape
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &dynamicBox;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.3f;
    fixtureDef.filter.categoryBits = playerBits;
    
    m_body->CreateFixture(&fixtureDef);
    //--sides fixture
    dynamicBox.SetAsBox(size.x / (4.f * SCALE)+0.01f, size.y / (2.f * SCALE)-0.02f);
    fixtureDef.friction = 0;
    fixtureDef.filter.maskBits = wallBits | movingBlockBits | boundryBits | fallingBlockBits | giftBits;
    m_body->CreateFixture(&fixtureDef);
    //set up sensor
    dynamicBox.SetAsBox((1 / SCALE)/2, size.y / (2.f * SCALE));
    //fixtureDef.friction = 0;
    fixtureDef.isSensor = true;
    fixtureDef.filter.categoryBits = playerSensorBits;
    fixtureDef.filter.maskBits = ladderBits | checkPointBits;
    m_body->CreateFixture(&fixtureDef);
    m_body->SetUserData(this);
    // add foot sensor fixture
    dynamicBox.SetAsBox(size.x/(SCALE*4), 1 / (SCALE * 2), b2Vec2(0, size.y / (2.f * SCALE)), 0);
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
    int pos = animPos;
    if (m_onRope) {
        m_body->SetLinearVelocity({ 0.f, 0.f });
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
        jump(dt);
        animPos = jumping;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
        m_direction = right;
        animPos = walking;
        if (m_onRope)
            return;
        else
            m_body->SetLinearVelocity({ dt * 75.f, m_body->GetLinearVelocity().y });
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
        m_direction = left;
        animPos = walking;
        if (m_onRope)
            return;
        else
            m_body->SetLinearVelocity({ -75.f * dt, m_body->GetLinearVelocity().y });
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
        m_direction = up;
        animPos = walking;
        if (m_onRope)
            m_body->SetLinearVelocity({ 0.f, -75.f * dt });
    }
    else if (m_onRope && (sf::Keyboard::isKeyPressed(sf::Keyboard::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Down))) {
        m_direction = down;
        animPos = walking;
        m_body->SetLinearVelocity({ 0.f, dt * 75.f });
    }

    else
        animPos = idle;
    if(m_onRope)
        animPos = climb;
    else if (m_numFootContact == 0) {
        animPos = jumping;
    }
    if(animPos!=pos){
        m_col = 0;
    }
}

void Player::jump(float dt) {
    //float impulse = -m_body->GetMass() * 9500*dt;
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

int Player::getDirection()
{
    return m_direction;
}

void Player::footContact(int val) {
    m_numFootContact += val;
}

void Player::updateAnim(float deltaTime) {
    updateRow();
    m_sprite.setTextureRect(Animation::getAnimRef().updateAnim(m_row, m_col,
        deltaTime, m_totalTime, player,m_direction));

}

void Player::fallDown() {
    sf::Vector2f vec(m_checkPoint.x/SCALE, m_checkPoint.y/SCALE);
    //sf::Vector2f vec(50/SCALE, 50/SCALE);
    m_body->SetTransform({ vec.x,vec.y }, m_body->GetAngle());
   //m_body->GetLocalPoint(m_body->GetWorldPoint()).Set(vec.x,vec.y);
    m_sprite.setPosition(m_checkPoint);
    //m_sprite.setPosition(50,50);
}

void Player::setCheckPoint(const sf::Vector2f& cp){
    m_checkPoint=cp;
}

void Player::updateRow() {
    switch (animPos) {
    case idle:
        m_row = 0;
        break;
    case walking:
        m_row = 1;
        break;
    case jumping:
        m_row = 2;
        break;
    case climb:
        m_row = 3;
    }
}