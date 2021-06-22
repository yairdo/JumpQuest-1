#include "box2d/box2d.h"
#include "iostream"
class ContactListner : public b2ContactListener
{
public:
    void BeginContact(b2Contact* contact);
    void PreSolve(b2Contact* contact, const b2Manifold*) override;
    void EndContact(b2Contact* contact);

private:
    bool checkFootContact(int , int , b2Body*);
    bool checkIfHitBoundry(b2Contact* contact) const;

    void handleCollision(b2Body* body1, b2Body* body2);
    bool movingBlockSolve(b2Contact* contact,float,bool);
    bool checkNoHandle(b2Contact*) const;
    bool m_preSolved = false;

};