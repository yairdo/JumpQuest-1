#include "Player.h"
#include "box2d/box2d.h"
#include "Macros.h"
#include <SFML/Graphics.hpp>
#include <NetworkObject.h>
Player::Player(b2World& world, const sf::Vector2f& pos, const sf::Vector2f& size,
    int bodyType, int id, Board& board) :
    MovingObj(world, pos, size, b2_dynamicBody, PLAYER_WIDTH, PLAYER_HEIGHT, player0 + id, castle),
    m_numFootContact(0), m_checkPoint(pos), m_gotGift(false),
    m_projectileForce({ 0,0 }), m_board(&board), m_win(false), m_moving(false), m_stuned(false), m_stunTime(0)
{
  //  m_sprite.setOrigin(m_sprite.getTextureRect().width / 2.f, m_sprite.getTextureRect().height / 2.f); 
  //  m_sprite.setColor(sf::Color::Green);
    /*m_sprite.setTextureRect(sf::IntRect(0, 0, PLAYER_WIDTH, PLAYER_HEIGHT));
    m_sprite.setScale(size.x / m_sprite.getGlobalBounds().width, size.y / m_sprite.getGlobalBounds().height);
    m_sprite.setOrigin(m_sprite.getTextureRect().width / 2.f, m_sprite.getTextureRect().height / 2.f);*/
    m_body->SetFixedRotation(true);
    //b2PolygonShape dynamicBox;
    //dynamicBox.SetAsBox(size.x/(4.f*SCALE), size.y / (2.f * SCALE));
    //set player shape
    
    /*b2FixtureDef fixtureDef;
    fixtureDef.shape = &dynamicBox;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.3f;
    fixtureDef.filter.categoryBits = playerBits;*/
    //m_body->CreateFixture(&fixtureDef);

    //--sides fixture
    /*dynamicBox.SetAsBox(size.x / (4.f * SCALE)+0.01f, size.y / (2.f * SCALE)-0.02f);
    createFixtureDef(dynamicBox, 1.0f, 0.f, noneBit,false,~ladderBits);*/

    //fixtureDef.friction = 0;
    //fixtureDef.filter.categoryBits = noneBit;;

    //fixtureDef.filter.maskBits = 0xFFFF & ~ladderBits;//wallBits | movingBlockBits | boundryBits | fallingBlockBits | giftBits;
    //m_body->CreateFixture(&fixtureDef);
    
    //set up sensor
    
    //fixtureDef.friction = 0;
    //fixtureDef.isSensor = true;
    //fixtureDef.filter.categoryBits = playerSensorBits;
    //fixtureDef.filter.maskBits = ladderBits | checkPointBits;
    //m_body->CreateFixture(&fixtureDef);
    b2PolygonShape dynamicBox(std::move(createPolygonShape({ (1 / SCALE) / 2, size.y / (2.f * SCALE) })));

    dynamicBox.SetAsBox((1 / SCALE) / 2, size.y / (2.f * SCALE));
    createFixtureDef(dynamicBox, 1.0f, 0.3f, playerSensorBits, true, ladderBits | checkPointBits);

    m_body->SetUserData(this);
    // add foot sensor fixture
    
    //fixtureDef.isSensor = true;
    //fixtureDef.filter.categoryBits = footBits;
    //fixtureDef.filter.maskBits = 0xFFFF;

    dynamicBox.SetAsBox((size.x) / (SCALE * 6), 1 / (SCALE * 2), b2Vec2(0, size.y / (2.f * SCALE)), 0);
    
    
    b2Fixture* footSensorFixture = createFixtureDef(dynamicBox, 1.0f, 0.3f, footBits, true,~(noHandleBit | noneBit | fallingBlockBits));
    footSensorFixture->SetUserData((void*)FOOT);

    dynamicBox.SetAsBox(size.x / (4.f * SCALE), size.y / (2.f * SCALE));
    //b2PolygonShape dynamicBox(std::move(createPolygonShape({ size.x / (4.f * SCALE), size.y / (2.f * SCALE) })));
    createFixtureDef(dynamicBox, 1.0f, 0.f, playerBits);
}

void Player::collectGift(){
    m_gotGift = true;
}

//updates player velocity according to which key is pressed
//applies impulse to jump
void Player::updatePhysics(float dt)
{
    static float m_timer = 0;
    bool moved = false;
    m_timer -= dt;
    if (m_stuned && m_stunTime >= STUN_TIME)
        m_stuned = false;
    if (getReset())
        reset();
    if (m_projectileForce != b2Vec2({ 0,0 })) {
        m_body->ApplyLinearImpulseToCenter(m_projectileForce, true);
        m_timer = 0.3;
        m_projectileForce = { 0,0 };
    }
    if (m_moving && m_numFootContact==0 && m_pushDown) {
        m_body->SetLinearVelocity({ 0,2 });
    }
    int pos = animPos;
    int dir = m_direction;
    m_direction = none;
    if (m_onRope && m_timer <= 0) {
        m_body->SetLinearVelocity({ 0.f, 0.f });
    }
    if (!m_stuned) {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && m_timer <= 0) {
            jump(dt);
            animPos = jumping;
            moved = true;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
            if (m_onRope)
                return;
            else if (m_timer <= 0)
                m_body->SetLinearVelocity({ dt * 75.f, m_body->GetLinearVelocity().y });
            m_direction = right;
            animPos = walking;
            moved = true;
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
            if (m_onRope)
                return;
            else if (m_timer <= 0)
                m_body->SetLinearVelocity({ -75.f * dt, m_body->GetLinearVelocity().y });
            m_direction = left;
            animPos = walking;
            moved = true;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
            if (m_canCatch && !m_onRope && m_timer <= 0) {
                m_body->SetTransform({ m_offSet.x / SCALE, getPos().y / SCALE }, 0);
                setOnRope(true);
            }
            if (m_onRope && m_timer <= 0) {
                m_body->SetLinearVelocity({ 0.f, -75.f * dt });
                moved = true;
            }
        }
        else if (m_onRope && m_timer <= 0 && (sf::Keyboard::isKeyPressed(sf::Keyboard::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Down))) {
            m_body->SetLinearVelocity({ 0.f, dt * 75.f });
            moved = true;
        }
    }
    else {
        m_stunTime += dt;
        animPos = stun;
    }
    if (!moved && m_numFootContact && m_timer <= 0 && !m_moving)
        m_body->SetLinearVelocity({ 0, m_body->GetLinearVelocity().y });

    if (m_direction == none&& !m_stuned) {
        animPos = idle;
        m_direction = dir;
    }
    
    if (m_onRope && !m_canCatch)//fell off rope
        setOnRope(false);
    
    if(m_onRope)
        animPos = climb;
    else if (m_numFootContact == 0 && !m_moving) {
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
            m_body->ApplyForce(b2Vec2(impulse/2, impulse/2), m_body->GetWorldCenter(), true);
            setOnRope(false);
            Resources::getResourceRef().playSound(jumpingSound);
        }
        else if(sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right)){
            m_body->ApplyForce(b2Vec2(-impulse/2, impulse/2), m_body->GetWorldCenter(), true);
            setOnRope(false);
            Resources::getResourceRef().playSound(jumpingSound);
        }
        return;
    }
    if (m_numFootContact) {
        m_body->ApplyForce(b2Vec2(0, impulse), m_body->GetWorldCenter(), true);
        Resources::getResourceRef().playSound(jumpingSound);
    }
}

void Player::move()
{
    auto position = m_body->GetPosition();
    m_sprite.setPosition(position.x * SCALE, position.y * SCALE);
    m_name.setPosition(m_sprite.getPosition().x, m_sprite.getPosition().y - PLAYER_SIZE.y / 1.5f);
    //for (auto& projs : m_projectile) {
    //    projs->move();
    //}
}

void Player::draw(sf::RenderWindow& window)
{
    window.draw(m_sprite);
    window.draw(m_name);
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
        deltaTime, m_totalTime, player0,m_direction));

}

void Player::reset() {
    sf::Vector2f vec(m_checkPoint.x/SCALE, m_checkPoint.y/SCALE);
    //sf::Vector2f vec(50/SCALE, 50/SCALE);
    m_body->SetTransform({ vec.x,vec.y }, m_body->GetAngle());
   //m_body->GetLocalPoint(m_body->GetWorldPoint()).Set(vec.x,vec.y);
    m_sprite.setPosition(m_checkPoint);
    //m_sprite.setPosition(50,50);
    Resources::getResourceRef().playSound(teleportSound);
    m_stuned = true;
    m_stunTime = 0;
    m_body->SetLinearVelocity({0,0});
    setReset(false);
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
        break;
    case stun:
        m_row = 6;
        break;

    }
}

void Player::center(const sf::Vector2f& ropePos) {
    m_offSet = ropePos;
}

void Player::setName(const std::string& name) {
    m_name.setFont(Resources::getResourceRef().getFont(lobbyFont));
    m_name.setString(name);
    m_name.setOrigin(m_name.getGlobalBounds().width / 2, m_name.getGlobalBounds().height / 2);
    m_name.setScale(PLAYERS_NAMES_SCALE);
}
void Player::useGift(const sf::Vector2f& mousePos, NetworkObject* network) {
    
    if (m_gotGift) {
        Resources::getResourceRef().playSound(rocketSound);
        if (network)
            network->addProjectile(AddProjectileMessage(m_sprite.getPosition(), mousePos,
                { m_sprite.getGlobalBounds().width,m_sprite.getGlobalBounds().height }));
        else
            m_board->addProjectile(AddProjectileMessage(m_sprite.getPosition(), mousePos,
                { m_sprite.getGlobalBounds().width,m_sprite.getGlobalBounds().height }));
    }
}
void Player::setExternalForce(const b2Vec2& force)
{
    m_projectileForce = force;
}

void Player::setMoving(bool vel)
{
    m_moving = vel;
}

void Player::winGame() {
    //std::cout << "ALLLHAAAA WACABARRRR $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$\n$$$$$$$$$$$$$$$$$$$\n$$$$$$$$$$$$$$$$";
    m_win = true;
}

float Player::getWidth() {
    return m_sprite.getGlobalBounds().width;
}