#include <ContactListner.h>
#include "CollisionHandler.h"
#include <Player.h>
#include "Macros.h"

void ContactListner::BeginContact(b2Contact* contact)
{
    /*b2Fixture* fixtureA = contact->GetFixtureA();
    b2Fixture* fixtureB = contact->GetFixtureB();*/
    b2Body* body1 = contact->GetFixtureA()->GetBody();
    b2Body* body2 = contact->GetFixtureB()->GetBody();
    checkFootContact((int)contact->GetFixtureA()->GetUserData(), 1, body1);
    checkFootContact((int)contact->GetFixtureB()->GetUserData(), 1, body2);
    GameObj* a = static_cast<GameObj*>(body1->GetUserData());
    GameObj* b = static_cast<GameObj*>(body2->GetUserData());
    if (!a || !b) return;
    //a->handleCol(b);
    CollisionHandler::getRef().handleCollision(a, b);
}

void ContactListner::EndContact(b2Contact* contact)
{
    b2Body* body1 = contact->GetFixtureA()->GetBody();
    b2Body* body2 = contact->GetFixtureB()->GetBody();
    checkFootContact((int)contact->GetFixtureA()->GetUserData(), -1, body1);
    checkFootContact((int)contact->GetFixtureB()->GetUserData(), -1, body2);
    if (contact->GetFixtureA()->GetFilterData().categoryBits == ladderBits ||
        contact->GetFixtureB()->GetFilterData().categoryBits == ladderBits) {
        GameObj* a = static_cast<GameObj*>(body1->GetUserData());
        GameObj* b = static_cast<GameObj*>(body2->GetUserData());
        if (!a || !b) return;
        //a->handleCol(b);
        CollisionHandler::getRef().handleCollision(a, b);
    }
}

void ContactListner::checkFootContact(int fixtureUserData, int val, b2Body* body) {
    if (fixtureUserData == FOOT) {
        auto player = static_cast<Player*>(body->GetUserData());
        player->footContact(val);
    }
}