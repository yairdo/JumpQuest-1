#define _USE_MATH_DEFINES

#include "Projectile.h"
#include "box2d/box2d.h"
#include "Macros.h"
#include <SFML/Graphics.hpp>
#include <math.h>
//-----------------------------------------------------------------------------
/*
* the projectile c-tor
*/
Projectile::Projectile(b2World& world, const sf::Vector2f& size, int bodyType,float dis,int map) :
    MovingObj(world, { 0,0 }, size, bodyType, 100,100,arrow,map),
    m_shot(false), m_elapaseTime(0),m_distance(dis),m_activeAnim(false)
{
    b2PolygonShape shape(std::move(createPolygonShape({ (size.x / SCALE) / 2, (size.y / SCALE) / 2 })));
    createFixtureDef(shape, 1.0f,0.3,noneBit, true, playerBits);
    m_body->SetUserData(this);
    m_body->SetAwake(false);
    m_body->SetGravityScale(0.f);
}
//-----------------------------------------------------------------------------
/*
* this function input is a wanted place to shoot the arrow to
* calculates the direction vector to shoot the arrow by
* and calculates his angle to rotate it properly
*/
void Projectile::shot(const sf::Vector2f& toPos){
    m_shot = true;
    m_body->SetAwake(true);
    m_vel.x = (toPos.x- m_sprite.getPosition().x)/SCALE;
    m_vel.y = (toPos.y- m_sprite.getPosition().y)/SCALE;
    m_vel.Normalize();

    float angle;
    if(toPos.y<m_sprite.getPosition().y)
         angle = -acos(m_vel.x)*180/M_PI;
    else
         angle = acos(m_vel.x) * 180 / M_PI;
    m_body->SetTransform(m_body->GetPosition(), angle);
    m_activeAnim = true;
    
}
//-----------------------------------------------------------------------------
/*
* updates the Projectile velocity, if he reached his max distance
* sets the RemoveObj member to true so we will remove the object- 
* this happens only when a player shoots the arrow, the archers arrow
* are not getting deleted here.
*/
void Projectile::updatePhysics(float dt) {
    
    if (m_distance <= 0) {
        setRemoveObj(true);
    }
    if (m_shot)
    {
        m_elapaseTime += dt;
        m_body->SetLinearVelocity({ m_vel.x * 130 * dt, m_vel.y * 130 * dt });
    }
}
//-----------------------------------------------------------------------------
/*
* moves the Projectile
*/
void Projectile::move()
{
    auto spritePos = m_sprite.getPosition();
    auto position = m_body->GetPosition();
    auto rotation = m_body->GetAngle();
    m_sprite.setPosition(position.x * SCALE, position.y * SCALE);
    m_sprite.setRotation(rotation);
    m_distance -= b2Distance({ spritePos.x,spritePos.y }, { m_sprite.getPosition().x, m_sprite.getPosition().y });
}
//-----------------------------------------------------------------------------
//updates animation
void Projectile::updateAnim(float deltaTime) {
    if(m_activeAnim)
        m_sprite.setTextureRect(Animation::getAnimRef().updateAnim(m_row, m_col,
           deltaTime, m_totalTime, arrow, up,m_distance/(65*ARROW_LEN)));
}
//-----------------------------------------------------------------------------
//resets the Projectile to his starting position and also resets his other attributes
void Projectile::reset()
{
    m_col = 0;
    m_sprite.setTextureRect(sf::IntRect(0, 0, PROJECTILE_SIZE.x, PROJECTILE_SIZE.y));
    m_body->SetAwake(false);
    m_shot = false;
    m_activeAnim = false;
}
//-----------------------------------------------------------------------------
sf::Vector2f Projectile::getPos() const {
    return m_sprite.getPosition();
}
//-----------------------------------------------------------------------------
b2Vec2 Projectile::getForce(sf::Vector2f playerPos) const
{
    if (playerPos.x > getPos().x)
        return PROJECTILE_FORCE;
    return {-PROJECTILE_FORCE.x,PROJECTILE_FORCE.y};
}
//-----------------------------------------------------------------------------
//sets the arrow face direction to know at the start in which direction to face
void Projectile::setFace(int face){
    m_faceTo = face;
}
//-----------------------------------------------------------------------------
void Projectile::setShot(bool s) {
    m_shot = s;
}
//-----------------------------------------------------------------------------
float Projectile::getDis()  const {
    return m_distance;
}
//-----------------------------------------------------------------------------
void Projectile::setDis(float dis){
    m_distance = dis;
}
//-----------------------------------------------------------------------------
/*
* calculates from which position to shoot the arrow from, if the player click to his left
* we want to know to shoot the arrow from the left side of the player - same for other directions
*/
sf::Vector2f Projectile::getPosToShotFrom(const sf::Vector2f& mouse, const sf::Vector2f& loc, const sf::Vector2f& bounds) {

    if (loc.x < mouse.x - (bounds.x / 2)) {
        return { loc.x+5 + bounds.x / 2+PROJECTILE_SIZE.x/2,loc.y };
    }
    else if (loc.x > mouse.x + (bounds.x / 2)) {
        return { loc.x -5- bounds.x/2 -PROJECTILE_SIZE.x / 2,loc.y };
    }
    else {
        if (loc.y < mouse.y) {
            return { loc.x,loc.y + bounds.y/2+ PROJECTILE_SIZE.y / 2+10};
        }
        else {
            return { loc.x,loc.y - bounds.y / 2 - PROJECTILE_SIZE.y / 2-10};
        }
    }

}
//-----------------------------------------------------------------------------
void Projectile::setPosition(const sf::Vector2f& loc) {
    m_sprite.setPosition(loc);
    m_body->SetTransform({ loc.x / SCALE, loc.y / SCALE }, m_body->GetAngle());
}
//-----------------------------------------------------------------------------
//to sync the object with the other players
void Projectile::setInfo(const MovingObjInfo& info) {
    setPosition(info.m_loc);
}
