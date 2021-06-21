#pragma once
#include <MessagesStructs.h>

//Messages constructors
MemberInfo memberInfoCreator(unsigned short = 0, const sf::Vector2f & = { 0,0 }, int = 0, int = 0, int = 0, float = 0);
GameMember gameMemberCreator(const sf::IpAddress& ip = sf::IpAddress::None, unsigned short port = 0,
    const char name[PLAYER_NAME_LEN] = "", const MemberInfo & = memberInfoCreator());
AddMember addMemberCreator(unsigned short id, const char name[PLAYER_NAME_LEN]);
StaticObjInfo staticObjInfoCreator(unsigned short id = 0, int index = 0);
MovingObjMembersRoport testLocsCreator(const std::vector<MovingObjInfo>&);
StartMessage startMessageCreator(int theme, int level);
AddProjectileMessage addProjectileMessageCreator(const sf::Vector2f& from,
    const sf::Vector2f& to, const sf::Vector2f& bounds);
MovingObjInfo movingObjInfoCreator(const sf::Vector2f & = { 0,0 },
    float = 0, const b2Vec2 & = { 0 , 0 });