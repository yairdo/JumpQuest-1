#pragma once
#include <Macros.h>
#include <SFML/Network.hpp>

//Each messege struct has creator out of the struct to make the struct lighter.
//============================================================================
struct GameMember {
    sf::IpAddress m_memberIp = sf::IpAddress::None;
    unsigned short m_memberPort = 0;
    char m_name[PLAYER_NAME_LEN] = "";
    sf::Vector2f m_loc = { 0,0 };
    unsigned short m_id = 0;
    int m_state = 0;
};
GameMember gameMemberCreator(const sf::IpAddress& ip = sf::IpAddress::None, unsigned short port = 0, 
    const char name[PLAYER_NAME_LEN] = "", unsigned short id = 0, int state = 0);
//============================================================================
struct MemberInfo {
    unsigned short m_id = 0;
    sf::Vector2f m_loc = { 0 , 0 };
    int state = 0;
};
MemberInfo memberInfoCreator(unsigned short = 0, const sf::Vector2f& = { 0,0 }, int = 0);
//============================================================================
struct AddMember {
    unsigned short m_id = 0;
    char m_name[PLAYER_NAME_LEN] = "";
};
AddMember addMemberCreator(unsigned short id, const char name[PLAYER_NAME_LEN]);
//============================================================================
struct TestLocs {
    int m_size = 0;
    sf::Vector2f m_locs[MAX_OBJ_IN_LEVEL];
};
TestLocs testLocsCreator(const std::vector<sf::Vector2f>&);