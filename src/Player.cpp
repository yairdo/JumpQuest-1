#include "Player.h"
#include "box2d/box2d.h"
#include "Macros.h"
#include <SFML/Graphics.hpp>
#include <NetworkObject.h>
#include <Board.h>
//---------------------------------------------------------------------------------------------------------
/*
* player c-tor
*/
Player::Player(b2World& world, const sf::Vector2f& pos, const sf::Vector2f& size,
    int bodyType, int id, Board& board) :
    MovingObj(world, pos, size, b2_dynamicBody, PLAYER_WIDTH, PLAYER_HEIGHT, player0 + id, castle),
    m_numFootContact(0), m_checkPoint(pos), m_gotGift(false),m_onRope(false), m_canCatch(false),
    m_projectileForce({ 0,0 }), m_board(&board), m_win(false), m_moving(false), m_stuned(false), m_stunTime(0)
{
    m_activeTimer = 0;
    b2PolygonShape dynamicBox(std::move(createPolygonShape({ (1 / SCALE) / 2, size.y / (2.f * SCALE) })));
    //middle of body sensor for ropes
    dynamicBox.SetAsBox((1 / SCALE) / 2, size.y / (2.f * SCALE));
    createFixtureDef(dynamicBox, 1.0f, 0.3f, playerSensorBits, true, ladderBits | checkPointBits);
    dynamicBox.SetAsBox((size.x) / (SCALE * 6), 1 / (SCALE * 2), b2Vec2(0, size.y / (2.f * SCALE)), 0);
    //sensor for feet
    b2Fixture* footSensorFixture = createFixtureDef(dynamicBox, 1.0f, 0.3f, footBits, true,~(noHandleBit | noneBit | fallingBlockBits));
    footSensorFixture->SetUserData((void*)FOOT);
    //main fixture
    dynamicBox.SetAsBox(size.x / (4.f * SCALE), size.y / (2.f * SCALE));
    createFixtureDef(dynamicBox, 1.0f, 0.f, playerBits);
    m_body->SetFixedRotation(true);
    m_body->SetUserData(this);
}
//---------------------------------------------------------------------------------------------------------
/*
* sets the player gotGift to true when he picks up a gift
*/
void Player::collectGift(){
    m_gotGift = true;
}
//---------------------------------------------------------------------------------------------------------
//updates player velocity according to which key is pressed and other conditions that need to be taken 
//into accout in the next world step. updates animation 
void Player::updatePhysics(float dt)
{
    bool moved = false;
    m_activeTimer -= dt;
    if (m_stuned && m_stunTime >= STUN_TIME)
        m_stuned = false;
    if (getReset())
        reset();
    applyProjectileImpulse();
    velocityCorrection(m_moving && m_numFootContact == 0 && m_pushDown, { 0, 2 });
    int pos = animPos;
    int dir = m_direction;
    m_direction = none;
    velocityCorrection(m_onRope && m_activeTimer <= 0, { 0.f, 0.f });
    if (!m_stuned) {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && m_activeTimer <= 0) {
            jump(dt);
            animPos = jumping;
            moved = true;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
            if (!horizontalMove(1, moved, animPos, dt, right))
                return;
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
            if(!horizontalMove(-1, moved, animPos, dt, left))
                return;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
            if (m_canCatch && !m_onRope && m_activeTimer <= 0) {
                m_body->SetTransform({ m_offSet.x / SCALE, getPos().y / SCALE }, 0);
                setOnRope(true);
            }
            velocityCorrection(m_onRope && m_activeTimer <= 0, { 0.f, -PLAYER_SPEED * dt });

        }
        else if ((sf::Keyboard::isKeyPressed(sf::Keyboard::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Down))) {
            velocityCorrection(m_onRope && m_activeTimer <= 0, { 0.f, dt * PLAYER_SPEED });
        }
    }
    else {
        m_stunTime += dt;
        animPos = stun;
    }
    velocityCorrection(!moved && m_numFootContact && m_activeTimer <= 0 && !m_moving, { 0, m_body->GetLinearVelocity().y });

    if (m_onRope && !m_canCatch)//fell off rope
        setOnRope(false);
    updateAnimPos(dir, pos);
}
//---------------------------------------------------------------------------------------------------------
// updates animation postion
void Player::updateAnimPos(int dir, int pos) {
    if (m_direction == none && !m_stuned) {
        animPos = idle;
        m_direction = dir;
    }
    if (m_onRope)
        animPos = climb;
    else if (m_numFootContact == 0 && !m_moving) {
        animPos = jumping;
    }
    if (animPos != pos) {
        m_col = 0;
    }
}
//---------------------------------------------------------------------------------------------------------
// sets the velocity when moving horizontaly, return true when if move completed 
bool Player::horizontalMove(int dirX, bool& moved, int& animPos, float dt, int animDirec) {
    if (m_onRope)
        return false;
    velocityCorrection(m_activeTimer <= 0, { dirX * PLAYER_SPEED * dt, m_body->GetLinearVelocity().y });
    moved = true;
    m_direction = animDirec;
    animPos = walking;
    return true;
}
//---------------------------------------------------------------------------------------------------------
// Sets the velocity of the player to the velocity in permeters if the given condition is true
void Player::velocityCorrection(bool condition, b2Vec2 vel) {
    if(condition)
        m_body->SetLinearVelocity(vel);
}
//---------------------------------------------------------------------------------------------------------
//applies force to the body in order to jump
void Player::jump(float dt) {
     float force = -m_body->GetMass() * JUMP_FORCE;//const
    if (m_onRope) {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
            ropeJump(force, 1);
        }
        else if(sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right)){
            ropeJump(force, -1);
        }
        return;
    }
    if (m_numFootContact) {
        m_body->ApplyForce(b2Vec2(0, force), m_body->GetWorldCenter(), true);
        Resources::getResourceRef().playSound(jumpingSound);
    }
}
//---------------------------------------------------------------------------------------------------------
// applies force to body to jump off a rope
void Player::ropeJump(float force, int directionX) {
    m_body->ApplyForce(b2Vec2(directionX*force / 2, force / 2), m_body->GetWorldCenter(), true);
    setOnRope(false);
    Resources::getResourceRef().playSound(jumpingSound);
}
//---------------------------------------------------------------------------------------------------------
// if the player was hit by a projectile, applies the force of the projectile to the body
void Player::applyProjectileImpulse() {
    if (m_projectileForce != b2Vec2({ 0,0 })) {
        m_body->SetLinearVelocity({ 0.f, 0.f });
        m_body->ApplyLinearImpulseToCenter(m_projectileForce, true);
        m_activeTimer = PLAYER_PROJ_EFFECT;
        m_projectileForce = { 0,0 };
    }
}
//---------------------------------------------------------------------------------------------------------
// moves the sprite and player name to according tp the position on the physics body
void Player::move()
{
    auto position = m_body->GetPosition();
    m_sprite.setPosition(position.x * SCALE, position.y * SCALE);
    m_name.setPosition(m_sprite.getPosition().x, m_sprite.getPosition().y - PLAYER_SIZE.y / 1.5f);
}
//---------------------------------------------------------------------------------------------------------
// Draws the player and his name to the window
void Player::draw(sf::RenderWindow& window) const 
{
    window.draw(m_sprite);
    window.draw(m_name);
}
//---------------------------------------------------------------------------------------------------------
// receives if the player is getting on and off the rope, if getting on will disable gravity and zero out
// the velocity if getting off then reapplies gravity
void Player::setOnRope(bool val){
    m_onRope = val;
    if (m_onRope) {
        m_body->SetLinearVelocity({ 0.f, 0.f });
        m_body->SetGravityScale(0);
        return;
    }
    m_body->SetGravityScale(1);
}
//---------------------------------------------------------------------------------------------------------
int Player::getDirection() const
{
    return m_direction;
}
//---------------------------------------------------------------------------------------------------------
// adds the given value to the number of foot contacts
void Player::footContact(int val) {
    m_numFootContact += val;
}
//---------------------------------------------------------------------------------------------------------
// updates the animation
void Player::updateAnim(float deltaTime) {
    updateRow();
    m_sprite.setTextureRect(Animation::getAnimRef().updateAnim(m_row, m_col,
        deltaTime, m_totalTime, player0,m_direction));

}
//---------------------------------------------------------------------------------------------------------
// called upon falling or hitting a floor obstacle will return the player to the last checkpoint
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
//---------------------------------------------------------------------------------------------------------
void Player::setCheckPoint(const sf::Vector2f& cp){
    
    m_checkPoint=cp;
}
//---------------------------------------------------------------------------------------------------------
// updates the animation row
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
//---------------------------------------------------------------------------------------------------------
// saves the rope's position to center the player on it while climbing
void Player::center(const sf::Vector2f& ropePos) {
    m_offSet = ropePos;
}
//---------------------------------------------------------------------------------------------------------
// Sets the player name that appears above them in multiplier mode
void Player::setName(const std::string& name) {
    m_name.setFont(Resources::getResourceRef().getFont(lobbyFont));
    m_name.setString(name);
    m_name.setOrigin(m_name.getGlobalBounds().width / 2, m_name.getGlobalBounds().height / 2);
    m_name.setScale(PLAYERS_NAMES_SCALE);
}
//---------------------------------------------------------------------------------------------------------
// called when left mouse is clicked, if the player collected a gift then adds the gift to the board
// if in multiplier mode, tells server that a projectiole needs to be added to the board
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
//---------------------------------------------------------------------------------------------------------
// called when hit by projectile will save the force that needs to be applied next step
void Player::setExternalForce(const b2Vec2& force)
{
    m_projectileForce = force;
}
//---------------------------------------------------------------------------------------------------------
void Player::setMoving(bool vel)
{
    m_moving = vel;
}
//---------------------------------------------------------------------------------------------------------
void Player::winGame() {
    m_win = true;
}
//---------------------------------------------------------------------------------------------------------
float Player::getWidth() {
    return m_sprite.getGlobalBounds().width;
}