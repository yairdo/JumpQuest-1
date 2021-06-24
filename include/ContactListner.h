#include "box2d/box2d.h"
class ContactListner : public b2ContactListener
{
public:

    virtual ~ContactListner() = default;
    //----------Functions---------

    void BeginContact(b2Contact* contact);
    void EndContact(b2Contact* contact);

private:
	//----------Functions---------

    bool checkFootContact(int , int , b2Body*);
    bool checkIfHitBoundry(b2Contact* contact) const;
    void handleCollision(b2Body* body1, b2Body* body2) const;
    bool movingBlockSolve(b2Contact* contact,float,bool) const;
    bool checkNoHandle(b2Contact*) const;

    //---------Membmbers----------

    int m_footContacts = 0;
};