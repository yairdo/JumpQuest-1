#include "box2d/box2d.h"
#include "iostream"
class ContactListner : public b2ContactListener
{
public:
    void BeginContact(b2Contact* contact);
    void EndContact(b2Contact* contact);
private:
    void checkFootContact(int , int , b2Body*);

};