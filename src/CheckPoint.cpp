#include "CheckPoint.h"
#include "box2d/box2d.h"
#include "Macros.h"
#include <SFML/Graphics.hpp>
#include <Factory.h>

bool CheckPoint::m_registerit = Factory<StaticObj>::registerit("CheckPoint",
    [](b2World& world,int map, std::vector<sf::Vector2f> vec)-> std::unique_ptr<StaticObj>
    { return std::make_unique<CheckPoint>(world, vec[0], vec[1], b2_staticBody, map); });

CheckPoint::CheckPoint(b2World& world, const sf::Vector2f& pos, const sf::Vector2f& size,
    int bodyType,int mapEnum) :
    StaticObj(world, pos, size,bodyType, CHECKPOINT_WIDTH, CHECKPOINT_HEIGHT,checkPoint),
    m_activate(false) 
{

    /*m_sprite.setTextureRect(sf::IntRect(0, 0, CHECKPOINT_WIDTH, CHECKPOINT_HEIGHT));
    m_sprite.setScale(size.x / m_sprite.getGlobalBounds().width, size.y / m_sprite.getGlobalBounds().height);
    m_sprite.setOrigin(m_sprite.getTextureRect().width / 2.f, m_sprite.getTextureRect().height / 2.f);*/

    /*b2PolygonShape groundBox;
    groundBox.SetAsBox((size.x / SCALE) / 2, (size.y / SCALE) / 2);
    b2FixtureDef fixture;
    fixture.shape = &groundBox;
    fixture.friction = 0.f;
    fixture.filter.categoryBits = checkPointBits;
    fixture.filter.maskBits = playerSensorBits;

    m_body->CreateFixture(&fixture);*/
    b2PolygonShape groundBox(std::move(createPolygonShape({ (size.x / SCALE) / 2, (size.y / SCALE) / 2 })));
    createFixtureDef(groundBox, 0.f, 0.f, checkPointBits, false, playerSensorBits);

    m_body->SetUserData(this);
    m_row = 1;
}

void CheckPoint::draw(sf::RenderWindow& window)
{
    window.draw(m_sprite);
}

void CheckPoint::setColor(sf::Color color){
    m_sprite.setColor(color);
}

void CheckPoint::activate(){
    Resources::getResourceRef().playSound(checkPointSound);
    m_row = 0;
    m_activate = true;
}

bool CheckPoint::getActive() const
{
    return m_activate;
}

void CheckPoint::updateAnim(float deltaTime) {

    m_sprite.setTextureRect(Animation::getAnimRef().updateAnim(m_row, m_col,
        deltaTime, m_totalTime, checkPoint, up));
} 
