#include "Gift.h"
#include "box2d/box2d.h"
#include "Macros.h"
#include <Factory.h>

//for factory
bool Gift::m_registerit = Factory<StaticObj>::registerit("Gift",
    [](b2World& world,int map,std::vector<sf::Vector2f> vec) -> std::unique_ptr<StaticObj>
    { return std::make_unique<Gift>(world, vec[0], vec[1], b2_staticBody,map); });
//-----------------------------------------------------------------------------
//the gift object c-tor
Gift::Gift(b2World& world, const sf::Vector2f& pos, const sf::Vector2f& size, int bodyType,int mapEnum) : 
    StaticObj(world, pos, size, bodyType, GIFT_WIDTH, GIFT_HEIGHT, gift,mapEnum)
{
    b2PolygonShape groundBox(std::move(createPolygonShape({ (size.x / SCALE) / 2, (size.y / SCALE) / 2 })));
    createFixtureDef(groundBox, 0.f, 1.f, giftBits, false, ~noneBit);
    m_body->SetUserData((GameObj*)this);
}
//-----------------------------------------------------------------------------
//decrease the gift counter when a player collides with the gift
//when the counter reaches 0 , the gift is being collected\removes
bool Gift::collisionCounter(){
    if (!--m_counter) {
        this->setRemoveObj(true);
        Resources::getResourceRef().playSound(giftCollectSound);
        return true;
    }
    else
        Resources::getResourceRef().playSound(giftHitSound);
    m_sprite.setTextureRect(Animation::getAnimRef().updateAnim(m_row, m_col,
       1, m_totalTime, gift, up,1));
    return false;
}
//-----------------------------------------------------------------------------
//sends a MSG to the other players so they will know a player hit
// the gift
void Gift::MsgCollision(){
    collisionCounter();
}

