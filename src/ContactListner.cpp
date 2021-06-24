#include <ContactListner.h>
#include "CollisionHandler.h"
#include <Player.h>
#include "Macros.h"
//-----------------------------------------------------------------------------
/*
this function is responsible on the behavior of the begin of the contact
between 2 objects 
*/
void ContactListner::BeginContact(b2Contact* contact)
{
    auto body1 = contact->GetFixtureA()->GetBody();
    auto body2 = contact->GetFixtureB()->GetBody();
    if (checkFootContact((int)contact->GetFixtureA()->GetUserData(), 1, body1) ||
        checkFootContact((int)contact->GetFixtureB()->GetUserData(), 1, body2))
        return;
    if (checkNoHandle(contact))
        return;
    if (movingBlockSolve(contact, MOVING_BLOCK_FRICTION, true))
        return;
    if (checkIfHitBoundry(contact))
        return;
    handleCollision(body1, body2);
}
//-----------------------------------------------------------------------------
/*
this function is responsible on the behavior of the end of the contact
between 2 objects 
*/
void ContactListner::EndContact(b2Contact* contact)
{
    b2Body* body1 = contact->GetFixtureA()->GetBody();
    b2Body* body2 = contact->GetFixtureB()->GetBody();
    if (checkFootContact((int)contact->GetFixtureA()->GetUserData(), -1, body1) ||
        checkFootContact((int)contact->GetFixtureB()->GetUserData(), -1, body2))
        return;
    movingBlockSolve(contact, 0, true);
    if (contact->GetFixtureA()->GetFilterData().categoryBits == ladderBits ||
        contact->GetFixtureB()->GetFilterData().categoryBits == ladderBits ||
        ((contact->GetFixtureA()->GetFilterData().categoryBits |
        contact->GetFixtureB()->GetFilterData().categoryBits) == (fallingBlockBits | playerBits))){ 
        handleCollision(body1, body2);
    }
}
//-----------------------------------------------------------------------------
/*
this function checks if the "Feets" of the player are having contact 
with another object
*/
bool ContactListner::checkFootContact(int fixtureUserData, int val, b2Body* body) {
    if (fixtureUserData == FOOT) {
        auto player = static_cast<Player*>(body->GetUserData());
        player->footContact(val);
        m_footContacts += val;
        return true;
    }
    return false;
}
//-----------------------------------------------------------------------------
/*This function handles the contact between a physics object and the map's 
 bottom boundry, will set the object up to rest to checkpoint or starting 
 position*/

bool ContactListner::checkIfHitBoundry(b2Contact* contact) const {
    if (contact->GetFixtureA()->GetFilterData().categoryBits == boundryBits) {
        auto obj = static_cast<MovingObj*>(contact->GetFixtureB()->GetBody()->GetUserData());
        obj->setReset(true);
    }
    else if (contact->GetFixtureB()->GetFilterData().categoryBits == boundryBits) {
        auto obj = static_cast<MovingObj*>(contact->GetFixtureA()->GetBody()->GetUserData());
        obj->setReset(true);
    }
    else
        return false;
    return true;
}
//-----------------------------------------------------------------------------
/*
this function calls the Collision Handler with the 2 objects that collided
*/
void ContactListner::handleCollision(b2Body* body1, b2Body* body2) const{
    GameObj* obj1 = static_cast<GameObj*>(body1->GetUserData());
    GameObj* obj2 = static_cast<GameObj*>(body2->GetUserData());
    if (!obj1 || !obj2) return;
    CollisionHandler::getRef().handleCollision(obj1, obj2);
}
//-----------------------------------------------------------------------------
/*This function handles the contact of the player with a moving block, will
  set the contact friction to a value in the perametes and calls collision handler*/
bool ContactListner::movingBlockSolve(b2Contact* contact, float friction, bool enter) const
{
    if (contact->GetFixtureA()->GetFilterData().categoryBits == movingBlockBits ||
        contact->GetFixtureB()->GetFilterData().categoryBits == movingBlockBits) {
        contact->SetFriction(friction);
        handleCollision(contact->GetFixtureA()->GetBody(), contact->GetFixtureB()->GetBody());
    }
    else
        return false;
    return true;
}
//-----------------------------------------------------------------------------
// checks if the category bits that need contact but no collision handling
bool ContactListner::checkNoHandle(b2Contact* contact) const{
    if (contact->GetFixtureA()->GetFilterData().categoryBits == noHandleBit ||
        contact->GetFixtureB()->GetFilterData().categoryBits == noHandleBit) {
        return true;
    }
    return false;
}
