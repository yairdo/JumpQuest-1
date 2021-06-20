#include "Block.h"
#include "box2d/box2d.h"
#include "Macros.h"
#include <SFML/Graphics.hpp>
#include <Factory.h>

bool Block::m_registerit = Factory<StaticObj>::registerit("Block",
    [](b2World& world,int mapEnum,std::vector<sf::Vector2f> vec)-> std::unique_ptr<StaticObj>
    { return std::make_unique<Block>(world, vec[0], vec[1], b2_staticBody,mapEnum); });

Block::Block(b2World& world, const sf::Vector2f& pos, const sf::Vector2f& size,
    int bodyType, int mapEnum): 
    StaticObj(world, pos, size, bodyType,block,mapEnum)
{
    m_sprite.setScale(size.x / m_sprite.getGlobalBounds().width, size.y / m_sprite.getGlobalBounds().height);
    m_sprite.setOrigin(m_sprite.getTextureRect().width / 2.f, m_sprite.getTextureRect().height / 2.f);
    m_sprite.setColor(sf::Color::White);

    /*b2PolygonShape groundBox;
    groundBox.SetAsBox((size.x/SCALE)/2, (size.y / SCALE) / 2);
    b2FixtureDef fixture;
    fixture.shape = &groundBox;
    fixture.friction = 1.f;
    fixture.filter.categoryBits = wallBits;

    m_body->CreateFixture(&fixture);*/
    b2PolygonShape groundBox(std::move(createPolygonShape({ (size.x / SCALE) / 2, (size.y / SCALE) / 2 })));
    createFixtureDef(groundBox, 0.f, 1.f, wallBits);

    
    //m_body->SetUserData(this);
}

void Block::draw(sf::RenderWindow& window)
{
    window.draw(m_sprite);
}
void Block::updateAnim(float deltaTime) {

    //m_sprite.setTextureRect(Animation::getAnimRef().updateAnim(0, m_col, deltaTime,m_totalTime, block,idle));

}
