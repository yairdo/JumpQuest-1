#define _USE_MATH_DEFINES

#include "Projectile.h"
#include "box2d/box2d.h"
#include "Macros.h"
#include <SFML/Graphics.hpp>
#include <math.h>

Projectile::Projectile(b2World& world, const sf::Vector2f& size, int bodyType,float dis) :
    MovingObj(world, { 0,0 }, size, bodyType), m_shot(false), m_elapaseTime(0),m_distance(dis)
{
    m_sprite.setColor(sf::Color::Yellow);
    m_sprite.setScale(size.x / m_sprite.getGlobalBounds().width, size.y / m_sprite.getGlobalBounds().height);
	m_sprite.setOrigin(m_sprite.getTextureRect().width / 2.f, m_sprite.getTextureRect().height / 2.f);
    /*b2PolygonShape kinematic;
    kinematic.SetAsBox((size.x / SCALE) / 2, (size.y / SCALE) / 2);

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &kinematic;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.3f;*/
    b2PolygonShape kinematic(std::move(createPolygonShape({ (size.x / SCALE) / 2, (size.y / SCALE) / 2 })));
    createFixtureDef(kinematic, 1.0f,0.3);

    //fixtureDef.filter.categoryBits = fallingBlockBits;

    //m_body->CreateFixture(&fixtureDef);

    m_body->SetUserData(this);
    m_body->SetAwake(false);
    m_body->SetGravityScale(0.f);
}

void Projectile::shot(const sf::Vector2f& toPos){
    m_shot = true;
    m_body->SetAwake(true);
    //float angle = 30;
    //float targetDist = b2Distance({fromPos.x/SCALE, fromPos.y/SCALE}, { toPos.x / SCALE, toPos.y / SCALE });
    //float projectileVel = targetDist / (sin(2 * angle * M_PI/180) / m_body->GetWorld()->GetGravity().y);
    //m_vel.x = sqrt(projectileVel) * cos(angle * M_PI / 180);
    //m_vel.y = sqrt(projectileVel) * sin(angle * M_PI / 180);
   /* float time = 1;

    float y = -((toPos.y - fromPos.y)-(0.5 * m_body->GetWorld()->GetGravity().y * (time) * (time)))/(time);
    float x = (toPos.x - fromPos.x) / time;
    m_vel.x = x;
    m_vel.y = y;*/
    //m_vel = { toPos.x - fromPos.x, toPos.y - fromPos.y };
    m_vel.x = (toPos.x- m_sprite.getPosition().x)/SCALE;
    m_vel.y = (toPos.y- m_sprite.getPosition().y)/SCALE;
    m_vel.Normalize();
}

void Projectile::updatePhysics(float dt) {
    
    if (m_shot)
    {
        m_elapaseTime += dt;
        std::cout << "yo yo im here\n";
        m_body->SetLinearVelocity({ m_vel.x * 220 * dt, m_vel.y * 220 * dt });
        //m_body->SetLinearVelocity({ m_vel.x*dt, -(m_vel.y - (m_body->GetWorld()->GetGravity().y * m_elapaseTime))*dt});
       // m_body->ApplyForceToCenter({ m_vel.x, m_vel.y}, true);
      //  m_body->ApplyForceToCenter({m_vel.x, -(m_vel.y - (m_body->GetWorld()->GetGravity().y * m_elapaseTime))}, true);
       // m_shot = false;
        return;
    }
 /*   if (m_shot && !m_body->IsAwake())
        reset();*/
   // if (!m_body->IsAwake())
       // reset();
}
void Projectile::move()
{
    auto spritePos = m_sprite.getPosition();
    auto position = m_body->GetPosition();
    auto rotation = m_body->GetAngle();
    m_sprite.setPosition(position.x * SCALE, position.y * SCALE);
    m_sprite.setRotation(rotation);
    m_distance -= b2Distance({ spritePos.x,spritePos.y }, { m_sprite.getPosition().x, m_sprite.getPosition().y });
}

void Projectile::draw(sf::RenderWindow& window)
{
    window.draw(m_sprite);
}

void Projectile::reset()
{
   // m_body->SetTransform({ m_strtPos.x, m_strtPos.y }, 0);
    m_body->SetAwake(false);
    m_shot = false;
}

sf::Vector2f Projectile::getPos() {
    return m_sprite.getPosition();
}

void Projectile::setShot(bool s) {
    m_shot = s;
}

float Projectile::getDis()  const {
    return m_distance;
}

void Projectile::setDis(float dis){
    m_distance = dis;
}

sf::Vector2f Projectile::getPosToShotFrom(const sf::Vector2f& mouse, const sf::Vector2f& loc, const sf::Vector2f& bounds) {

    if (loc.x < mouse.x - bounds.x / 2) {
        return { loc.x + bounds.x / 2+PROJECTILE_SIZE.x/2,loc.y };
    }
    else if (loc.x > mouse.x + bounds.x / 2) {
        return { loc.x - bounds.x -PROJECTILE_SIZE.x / 2 / 2,loc.y };
    }
    else {
        if (loc.y < mouse.y) {
            return { loc.x,loc.y + bounds.y+ PROJECTILE_SIZE.y / 2 / 2 };
        }
        else {
            return { loc.x,loc.y - bounds.y / 2 - PROJECTILE_SIZE.y / 2 };
        }
    }
}