#include "Player.h"
#include "box2d/box2d.h"
#include "Macros.h"
#include <SFML/Graphics.hpp>
#include <NetworkObject.h>
Player::Player(b2World& world, const sf::Vector2f& pos, const sf::Vector2f& size,
    int bodyType, int id, Board& board) :
    MovingObj(world, pos, size, b2_dynamicBody, PLAYER_WIDTH, PLAYER_HEIGHT, player0 + id, castle),
    m_numFootContact(0), m_checkPoint(pos), m_gotGift(false),m_onRope(false), m_canCatch(false),
    m_projectileForce({ 0,0 }), m_board(&board), m_win(false), m_moving(false), m_stuned(false), m_stunTime(0)
{
    m_activeTimer = 0;
    b2PolygonShape dynamicBox(std::move(createPolygonShape({ (1 / SCALE) / 2, size.y / (2.f * SCALE) })));

    dynamicBox.SetAsBox((1 / SCALE) / 2, size.y / (2.f * SCALE));
    createFixtureDef(dynamicBox, 1.0f, 0.3f, playerSensorBits, true, ladderBits | checkPointBits);

    dynamicBox.SetAsBox((size.x) / (SCALE * 6), 1 / (SCALE * 2), b2Vec2(0, size.y / (2.f * SCALE)), 0);
    
    
    b2Fixture* footSensorFixture = createFixtureDef(dynamicBox, 1.0f, 0.3f, footBits, true,~(noHandleBit | noneBit | fallingBlockBits));
    footSensorFixture->SetUserData((void*)FOOT);

    dynamicBox.SetAsBox(size.x / (4.f * SCALE), size.y / (2.f * SCALE));
    //b2PolygonShape dynamicBox(std::move(createPolygonShape({ size.x / (4.f * SCALE), size.y / (2.f * SCALE) })));
    createFixtureDef(dynamicBox, 1.0f, 0.f, playerBits);
    m_body->SetFixedRotation(true);
    m_body->SetUserData(this);
}

void Player::collectGift(){
    m_gotGift = true;
}

//updates player velocity according to which key is pressed
//applies impulse to jump
void Player::updatePhysics(float dt)
{
    bool moved = false;
    /*m_timer*/ m_activeTimer -= dt;
    if (m_stuned && m_stunTime >= STUN_TIME)
        m_stuned = false;
    if (getReset())
        reset();
    applyProjectileImpulse();
    //if (m_projectileForce != b2Vec2({ 0,0 })) {
    //    m_body->SetLinearVelocity({ 0.f, 0.f });
    //    m_body->ApplyLinearImpulseToCenter(m_projectileForce, true);
    //    m_timer = 0.3;
    //    m_projectileForce = { 0,0 };
    //}
    velocityCorrection(m_moving && m_numFootContact == 0 && m_pushDown, { 0,2 });
    //if (m_moving && m_numFootContact == 0 && m_pushDown) {
    //    m_body->SetLinearVelocity({ 0,2 });
    //}
    int pos = animPos;
    int dir = m_direction;
    m_direction = none;
    velocityCorrection(m_onRope && m_activeTimer <= 0, { 0.f, 0.f });
    //if (m_onRope && m_activeTimer <= 0) {
    //    m_body->SetLinearVelocity({ 0.f, 0.f });
    //}
    if (!m_stuned) {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && m_activeTimer <= 0) {
            jump(dt);
            animPos = jumping;
            moved = true;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
            if (!horizontalMove(1, moved, animPos, dt, right))
                return;

            //if (m_onRope)
            //    return;
            //else if (m_activeTimer <= 0)
            //    m_body->SetLinearVelocity({ dt * PLAYER_SPEED, m_body->GetLinearVelocity().y });
            //m_direction = right;
            //animPos = walking;
            //moved = true;
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
            if(!horizontalMove(-1, moved, animPos, dt, left))
                return;
            //if (m_onRope)
            //    return;
            //else if (m_activeTimer <= 0)
            //    m_body->SetLinearVelocity({ -PLAYER_SPEED * dt, m_body->GetLinearVelocity().y });
            //m_direction = left;
            //animPos = walking;
            //moved = true;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
            if (m_canCatch && !m_onRope && m_activeTimer <= 0) {
                m_body->SetTransform({ m_offSet.x / SCALE, getPos().y / SCALE }, 0);
                setOnRope(true);
            }
            velocityCorrection(m_onRope && m_activeTimer <= 0, { 0.f, -PLAYER_SPEED * dt });
            //if (m_onRope && m_activeTimer <= 0) {
            //    m_body->SetLinearVelocity({ 0.f, -PLAYER_SPEED * dt });
            //    moved = true;
            //}
        }
        else if (/*m_onRope && m_activeTimer <= 0 && */(sf::Keyboard::isKeyPressed(sf::Keyboard::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Down))) {
            velocityCorrection(m_onRope && m_activeTimer <= 0, { 0.f, dt * PLAYER_SPEED });
            //m_body->SetLinearVelocity({ 0.f, dt * PLAYER_SPEED });
            //moved = true;
        }
    }
    else {
        m_stunTime += dt;
        animPos = stun;
    }
    velocityCorrection(!moved && m_numFootContact && m_activeTimer <= 0 && !m_moving, { 0, m_body->GetLinearVelocity().y });
    /*if (!moved && m_numFootContact && m_activeTimer <= 0 && !m_moving)
        m_body->SetLinearVelocity({ 0, m_body->GetLinearVelocity().y });*/

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

bool Player::horizontalMove(int dirX, bool& moved, int& animPos, float dt, int animDirec) {
    if (m_onRope)
        return false;
    velocityCorrection(m_activeTimer <= 0, { dirX * PLAYER_SPEED * dt, m_body->GetLinearVelocity().y });
   /* else if (m_activeTimer <= 0)
        m_body->SetLinearVelocity({ dirX*PLAYER_SPEED * dt, m_body->GetLinearVelocity().y });*/
    moved = true;
    m_direction = animDirec;
    animPos = walking;
    return true;
}

void Player::velocityCorrection(bool condition, b2Vec2 vel) {
    if(condition)
        m_body->SetLinearVelocity(vel);
}

void Player::jump(float dt) {
     float impulse = -m_body->GetMass() * 150;//const
    if (m_onRope) {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
            ropeJump(impulse, 1);
            //m_body->ApplyForce(b2Vec2(impulse/2, impulse/2), m_body->GetWorldCenter(), true);
            //setOnRope(false);
            //Resources::getResourceRef().playSound(jumpingSound);
        }
        else if(sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right)){
            ropeJump(impulse, -1);
            //m_body->ApplyForce(b2Vec2(-impulse/2, impulse/2), m_body->GetWorldCenter(), true);
            //setOnRope(false);
            //Resources::getResourceRef().playSound(jumpingSound);
        }
        return;
    }
    if (m_numFootContact) {
        m_body->ApplyForce(b2Vec2(0, impulse), m_body->GetWorldCenter(), true);
        Resources::getResourceRef().playSound(jumpingSound);
    }
}

void Player::ropeJump(float impulse, int directionX) {
    m_body->ApplyForce(b2Vec2(directionX*impulse / 2, impulse / 2), m_body->GetWorldCenter(), true);
    setOnRope(false);
    Resources::getResourceRef().playSound(jumpingSound);
}

void Player::applyProjectileImpulse() {
    if (m_projectileForce != b2Vec2({ 0,0 })) {
        m_body->SetLinearVelocity({ 0.f, 0.f });
        m_body->ApplyLinearImpulseToCenter(m_projectileForce, true);
        m_activeTimer = PLAYER_PROJ_EFFECT;
        m_projectileForce = { 0,0 };
    }
}

void Player::move()
{
    auto position = m_body->GetPosition();
    m_sprite.setPosition(position.x * SCALE, position.y * SCALE);
    m_name.setPosition(m_sprite.getPosition().x, m_sprite.getPosition().y - PLAYER_SIZE.y / 1.5f);
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

int Player::getDirection() const
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
    m_body->SetTransform({ vec.x,vec.y }, m_body->GetAngle());
    m_sprite.setPosition(m_checkPoint);
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
    m_win = true;
}

float Player::getWidth() {
    return m_sprite.getGlobalBounds().width;
}