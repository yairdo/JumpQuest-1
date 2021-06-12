#include "Projectile.h"
#include "box2d/box2d.h"
#include "Macros.h"
#include <SFML/Graphics.hpp>

Projectile::Projectile(b2World& world, const sf::Vector2f& startPos, const sf::Vector2f& size, const sf::Vector2f& force, int bodyType) :
    m_strtPos(startPos / SCALE), m_force(force), MovingObj(world, startPos, size, bodyType)
{
    m_sprite.setColor(sf::Color::Magenta);
    b2PolygonShape kinematic;
    kinematic.SetAsBox((size.x / SCALE) / 2, (size.y / SCALE) / 2);

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &kinematic;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.3f;
    //fixtureDef.filter.categoryBits = fallingBlockBits;

    m_body->CreateFixture(&fixtureDef);
    m_body->SetUserData((GameObj*)this);
    m_body->SetAwake(false);
}

void Projectile::shot(){
    m_shot = true;
}

void Projectile::updatePhysics(float dt) {
    if (m_shot)
    {
        m_body->ApplyForceToCenter({m_force.x,m_force.y}, true);
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