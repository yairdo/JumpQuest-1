#pragma once
#include <Macros.h>
#include "box2d/box2d.h"
#include <SFML/Network.hpp>

//Each message struct has creator out of the struct to make the struct lighter.
//============================================================================
struct MemberInfo {
unsigned short m_id = 0;
sf::Vector2f m_loc{ 50,50 };
int m_row = 0;
int m_col = 0;
int m_direction = 0;
//yair is trying stuff
float m_totalTime = 0;
};
MemberInfo memberInfoCreator(unsigned short=0, const sf::Vector2f & = {0,0}, int = 0, int = 0, int = 0,float=0);
//============================================================================
struct GameMember {
    sf::IpAddress m_memberIp = sf::IpAddress::None;
    unsigned short m_memberPort = 0;
    char m_name[PLAYER_NAME_LEN] = "";
    MemberInfo m_info;
};
GameMember gameMemberCreator(const sf::IpAddress& ip = sf::IpAddress::None, unsigned short port = 0,
    const char name[PLAYER_NAME_LEN] = "", const MemberInfo& = memberInfoCreator());
//============================================================================
struct AddMember {
    unsigned short m_id = 0;
    char m_name[PLAYER_NAME_LEN] = "";
};
AddMember addMemberCreator(unsigned short id, const char name[PLAYER_NAME_LEN]);
//============================================================================
struct MovingObjInfo {//add index?
    sf::Vector2f m_loc;
    float m_timer = 0;
    b2Vec2 m_vel = { 0,0 };
};
MovingObjInfo movingObjInfoCreator(const sf::Vector2f & = { 0,0 },
    float = 0, const b2Vec2 & = { 0 , 0 });
//============================================================================
struct StaticObjInfo {
    unsigned short m_id = 0;
    int m_index=0;
    //int m_counter=-1;
};
StaticObjInfo staticObjInfoCreator(unsigned short id=0,int index = 0);
//============================================================================
struct MovingObjMembersRoport {
    int m_size = 0;
    MovingObjInfo m_locs[MAX_OBJ_IN_LEVEL];
};
MovingObjMembersRoport testLocsCreator(const std::vector<MovingObjInfo>&);
//============================================================================
struct AddProjectileMessage {
    sf::Vector2f m_frome{ 0, 0 };
    sf::Vector2f m_to{ 0, 0 };
    sf::Vector2f m_bounds{ 0,0 };
};
AddProjectileMessage addProjectileMessageCreator(const sf::Vector2f& from,
    const sf::Vector2f& to, const sf::Vector2f& bounds);
//============================================================================
struct StartMessage {
    int m_theme = 0;
    int m_level = 0;
};
StartMessage startMessageCreator(int theme, int level);