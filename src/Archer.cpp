#include <Archer.h>
#include <Factory.h>
#include "..\include\Archer.h"

bool Archer::m_registerit = Factory<StaticObj>::registerit("Archer",
    [](b2World& world,int map, std::vector<sf::Vector2f> vec)-> std::unique_ptr<StaticObj>
    { return std::make_unique<Archer>(world, vec[0], vec[1],vec[2], b2_staticBody,map); });

Archer::Archer(b2World& world, const sf::Vector2f& pos,
    const sf::Vector2f& timerNDis, const sf::Vector2f& toPos, int bodyType,int mapEnum):m_distance(timerNDis.y),
    StaticObj(world, pos, ARCHER_SIZE, bodyType), m_timer(timerNDis.x) ,m_shotTO(toPos),
    m_proj(std::make_unique<Projectile>(world,PROJECTILE_SIZE , b2_dynamicBody,timerNDis.y)) {

    m_sprite.setScale(ARCHER_SIZE.x / m_sprite.getGlobalBounds().width, ARCHER_SIZE.y / m_sprite.getGlobalBounds().height);
    m_sprite.setOrigin(m_sprite.getTextureRect().width / 2.f, m_sprite.getTextureRect().height / 2.f);
    sf::Vector2f temp= m_proj->getPosToShotFrom(toPos, pos, { m_sprite.getGlobalBounds().width,m_sprite.getGlobalBounds().height });
    m_proj->setPos(temp);
    m_shotStartPos = temp;
    m_sprite.setColor(sf::Color::Blue);

    b2PolygonShape groundBox(std::move(createPolygonShape({ (ARCHER_SIZE.x / SCALE) / 2, (ARCHER_SIZE.y / SCALE) / 2 })));
    createFixtureDef(groundBox, 0.f, 1.f, giftBits, false, ~noneBit);

    m_body->SetUserData((GameObj*)this);
}

void Archer::updateAnim(float deltaTime) {
    static float time=m_timer;
    time -= deltaTime;
    //std::cout << time << "\n";
    if (time<=0) {
        time = m_timer;
        m_proj->shot(m_shotTO);
        //std::cout << "shot\n";
       // m_proj->updatePhysics(deltaTime);
    }

    if (m_proj->getShot()) {
        m_proj->updatePhysics(deltaTime);
        m_proj->move();
    }
    if (m_proj->getDis()<=0) {
        m_proj->setPos(m_shotStartPos);
        m_proj->setDis(m_distance);
        m_proj->reset();
    }
}

void Archer::draw(sf::RenderWindow& window) {
    window.draw(m_sprite);
    m_proj->draw(window);
}

