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
    if (checkFootContact((int)contact->GetFixtureA()->GetUserData(), 1, body1) ||
        checkFootContact((int)contact->GetFixtureB()->GetUserData(), 1, body2))
        return;
    if (checkIfHitBoundry(contact))
        return;
    GameObj* a = static_cast<GameObj*>(body1->GetUserData());
    GameObj* b = static_cast<GameObj*>(body2->GetUserData());
    if (a)
        std::cout <<"a= " <<typeid(*a).name() << std::endl;
    if (b)
        std::cout << "b= " << typeid(*b).name() << std::endl;
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

bool ContactListner::checkFootContact(int fixtureUserData, int val, b2Body* body) {
    if (fixtureUserData == FOOT) {
        auto player = static_cast<Player*>(body->GetUserData());
        player->footContact(val);
        return true;
    }
    return false;
}

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