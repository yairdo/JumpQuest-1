#pragma once
#include <Macros.h>
#include "box2d/box2d.h"
#include <SFML/Network.hpp>

//============================================================================
struct MemberInfo {
    MemberInfo(unsigned short = 0, const sf::Vector2f & = { 50,50 }, int = 0, 
        int = 0, int = 0, float = 0, bool =false);
    unsigned short m_id;
    sf::Vector2f m_loc;
    int m_row;
    int m_col;
    int m_direction;
    float m_totalTime;
    bool m_win;
};
//============================================================================
struct GameMember {
    GameMember(const sf::IpAddress& ip = sf::IpAddress::None, unsigned short port = 0,
        const char name[PLAYER_NAME_LEN] = "", const MemberInfo & = MemberInfo());
    sf::IpAddress m_memberIp;
    unsigned short m_memberPort;
    char m_name[PLAYER_NAME_LEN];
    MemberInfo m_info;
};
//============================================================================
struct AddMember {
    AddMember(unsigned short id = 0, const char name[PLAYER_NAME_LEN] = "");
    unsigned short m_id;
    char m_name[PLAYER_NAME_LEN];
};
//============================================================================
struct MovingObjInfo {
    MovingObjInfo(const sf::Vector2f & = { 0,0 },
        float = 0, const b2Vec2 & = { 0 , 0 },bool=0, unsigned short=0);
    sf::Vector2f m_loc;
    float m_timer;
    //int m_col;
    b2Vec2 m_vel;
    bool m_active;
    unsigned short m_index;
};
//============================================================================
struct StaticObjInfo {
    StaticObjInfo(unsigned short id = 0, int index = 0);
    unsigned short m_id;
    int m_index;
};
//============================================================================
struct MovingObjMembersRoport {
    MovingObjMembersRoport(const std::vector<MovingObjInfo> & = {});
    int m_size;
    MovingObjInfo m_locs[MAX_OBJ_IN_LEVEL];
};
//============================================================================
struct AddProjectileMessage {
    AddProjectileMessage(const sf::Vector2f& from = { 0,0 },
        const sf::Vector2f& to = { 0,0 }, const sf::Vector2f& bounds = { 0,0 });
    sf::Vector2f m_frome;
    sf::Vector2f m_to;
    sf::Vector2f m_bounds;
};
//============================================================================
struct StartMessage {
    StartMessage(int theme = 0, int level = 0);
    int m_theme;
    int m_level;
};