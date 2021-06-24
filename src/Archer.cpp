#include <Archer.h>
#include <Factory.h>

//-----------------------------------------------------------------------------
/*
    For factory
*/
bool Archer::m_registerit = Factory<MovingObj>::registerit("Archer",
    [](b2World& world,int map, std::vector<sf::Vector2f> vec)-> std::unique_ptr<MovingObj>
    { return std::make_unique<Archer>(world, vec[0], vec[1],vec[2], b2_staticBody,map); });


//-----------------------------------------------------------------------------
/*
    c--tor
*/
Archer::Archer(b2World& world, const sf::Vector2f& pos,
    const sf::Vector2f& timerNDis, const sf::Vector2f& toPos, int bodyType,int mapEnum):
    m_distance(timerNDis.y), m_timer(timerNDis.x), m_shotTO(toPos),
    MovingObj(world, pos, ARCHER_SIZE, bodyType, ARCHER_WIDTH, ARCHER_HEIGHT,archer,mapEnum),
    m_proj(std::make_unique<Projectile>(world,PROJECTILE_SIZE , b2_dynamicBody,timerNDis.y,mapEnum)) {

    sf::Vector2f temp= m_proj->getPosToShotFrom(toPos, pos, 
        { m_sprite.getGlobalBounds().width,m_sprite.getGlobalBounds().height });
    m_proj->setPos(temp);
    m_shotStartPos = temp;
    if (temp.x > m_sprite.getPosition().x) {
        m_faceTo = right; //for animation
    }
    else
        m_faceTo = left; //for animation
    m_proj->setFace(m_faceTo);
    m_activeTimer = m_timer; 
    b2PolygonShape groundBox(std::move(createPolygonShape
        ({ (ARCHER_SIZE.x / SCALE) / 2, (ARCHER_SIZE.y / SCALE) / 2 })));
    createFixtureDef(groundBox, 0.f, 1.f, giftBits, false, ~noneBit);

    m_body->SetUserData((GameObj*)this);
}
//-----------------------------------------------------------------------------
/*
    Function update anim.
    the function received delta time and update the animation.
*/
void Archer::updateAnim(float deltaTime) {
    
    m_sprite.setTextureRect(Animation::getAnimRef().updateAnim(m_row, m_col,
        deltaTime, m_totalTime, archer, m_faceTo));
    m_proj->updateAnim(deltaTime);
}
//-----------------------------------------------------------------------------
void Archer::draw(sf::RenderWindow& window)const {
    window.draw(m_sprite);
    m_proj->draw(window);
}
//-----------------------------------------------------------------------------
/*
    Function: update physics
    This function is updating the physics of the archer object.
*/
void Archer::updatePhysics(float dt) {
    m_timer -= dt;
    if (m_timer<=0) { //if archer needs to shot again
        m_timer = m_activeTimer;
        m_proj->shot(m_shotTO);
    }

    if (m_proj->getShot())
        m_proj->updatePhysics(dt);

    if (m_proj->getDis()<=0) { //if the arrow traveled its max distance - returns it to the start
        m_proj->setPosition(m_shotStartPos);
        m_proj->setDis(m_distance);
        m_proj->reset();
    }
}
//-----------------------------------------------------------------------------
/*
    Function: move
    This function moves the archers projectile body and sprite 
*/
void Archer::move() {
    if (m_proj->getShot())
         m_proj->move();
}
//-----------------------------------------------------------------------------
// this function sends the info to the other players to sync between the objects
MovingObjInfo Archer::getInfo() const
{
    return MovingObjInfo(m_proj->getPos(), m_timer, { float(m_proj->getShot()),m_proj->getDis()});
}

//-----------------------------------------------------------------------------
/*
    Function: set info 
    This function is updating the information of the object to sync
    between server and client.
*/
void Archer::setInfo(const MovingObjInfo& info){
    m_proj->setPosition(info.m_loc);
    m_timer = info.m_timer;
    m_proj->setShot(info.m_vel.x);
    m_proj->setDis(info.m_vel.y);
}
