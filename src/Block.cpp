#include "Block.h"
#include "box2d/box2d.h"
#include "Macros.h"
#include <SFML/Graphics.hpp>
#include <Factory.h>

//for factory
bool Block::m_registerit = Factory<StaticObj>::registerit("Block",
    [](b2World& world,int mapEnum,std::vector<sf::Vector2f> vec)-> std::unique_ptr<StaticObj>
    { return std::make_unique<Block>(world, vec[0], vec[1], b2_staticBody,mapEnum); });


//blocks c-tor
Block::Block(b2World& world, const sf::Vector2f& pos, const sf::Vector2f& size,
    int bodyType, int mapEnum): 
    StaticObj(world, pos, size, bodyType,0,0,block,mapEnum)
{
    b2PolygonShape groundBox(std::move(createPolygonShape({ (size.x / SCALE) / 2, (size.y / SCALE) / 2 })));
    createFixtureDef(groundBox, 0.f, 1.f, wallBits);

    
    m_body->SetUserData(this);
}

//block has no animation
void Block::updateAnim(float deltaTime) {


}
