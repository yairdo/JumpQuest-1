#define _USE_MATH_DEFINES

#include "Projectile.h"
#include "box2d/box2d.h"
#include "Macros.h"
#include <SFML/Graphics.hpp>
#include <math.h>

Projectile::Projectile(b2World& world, const sf::Vector2f& startPos, const sf::Vector2f& size, int bodyType) :
    m_strtPos(startPos / SCALE), MovingObj(world, startPos, size, bodyType), m_shot(false), m_elapaseTime(0)
{
    m_sprite.setColor(sf::Color::Yellow);
    m_sprite.setScale(size.x / m_sprite.getGlobalBounds().width, size.y / m_sprite.getGlobalBounds().height);
	m_sprite.setOrigin(m_sprite.getTextureRect().width / 2.f, m_sprite.getTextureRect().height / 2.f);
    b2PolygonShape kinematic;
    kinematic.SetAsBox((size.x / SCALE) / 2, (size.y / SCALE) / 2);

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &kinematic;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.3f;
    //fixtureDef.filter.categoryBits = fallingBlockBits;

    m_body->CreateFixture(&fixtureDef);
    m_body->SetUserData(this);
    m_body->SetAwake(false);
    m_body->SetGravityScale(0.f);
}

void Projectile::shot(const sf::Vector2f& fromPos, const sf::Vector2f& toPos){
    m_shot = true;
    m_body->SetAwake(true);
    /*
    float angle = 30;
    float targetDist = b2Distance({fromPos.x/SCALE, fromPos.y/SCALE}, { toPos.x / SCALE, toPos.y / SCALE });
    /*float projectileVel = targetDist / (sin(2 * angle * M_PI/180) / m_body->GetWorld()->GetGravity().y);
    m_vel.x = sqrt(projectileVel) * cos(angle * M_PI / 180);
    m_vel.y = sqrt(projectileVel) * sin(angle * M_PI / 180);
    */
    float time = 3;

    float y = ((toPos.y - fromPos.y)-(0.5 * m_body->GetWorld()->GetGravity().y * (time) * (time)))/(time);
    float x = (toPos.x - fromPos.x) / time;
    m_vel.x = x;
    m_vel.y = y;
}

void Projectile::updatePhysics(float dt) {
    if (m_shot)
    {
        //m_elapaseTime += dt;
        //m_body->SetLinearVelocity({ m_vel.x, -(m_vel.y - (m_body->GetWorld()->GetGravity().y * m_elapaseTime))});
        m_body->ApplyLinearImpulseToCenter({ 0,0 }, true);
        return;
    }
    if (m_shot && !m_body->IsAwake())
        reset();
}
void Projectile::move()
{
    auto position = m_body->GetPosition();
    auto rotation = m_body->GetAngle();
    m_sprite.setPosition(position.x * SCALE, position.y * SCALE);
    m_sprite.setRotation(rotation);
}

void Projectile::draw(sf::RenderWindow& window)
{
    window.draw(m_sprite);
}

void Projectile::reset()
{
    m_body->SetTransform({ m_strtPos.x, m_strtPos.y }, 0);
    m_body->SetAwake(false);
    m_shot = false;
}

sf::Vector2f Projectile::getPos() {
    return m_sprite.getPosition();
}