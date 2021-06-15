#include <MessegesStructs.h>
#include <memory>

GameMember gameMemberCreator(const sf::IpAddress& ip, unsigned short port, const char name[PLAYER_NAME_LEN], unsigned short id , int state ) {
    GameMember value;
    value.m_memberIp = ip;
    value.m_memberPort = port;
    value.m_id = id;
    value.m_state = state;
    std::memcpy(value.m_name, name, PLAYER_NAME_LEN);
    return value;
}

MemberInfo memberInfoCreator(unsigned short id, const sf::Vector2f& loc, int state) {
    MemberInfo value;
    value.m_id = id;
    value.m_loc = loc;
    value.state = state;
    return value;
}

AddMember addMemberCreator(unsigned short id, const char name[PLAYER_NAME_LEN]) {
    AddMember value;
    value.m_id = id;
    std::memcpy(value.m_name, name, PLAYER_NAME_LEN);
    return value;
}

TestLocs testLocsCreator(const std::vector<sf::Vector2f>& vec) {
    TestLocs value;
    value.m_size = vec.size();
    for (int i = 0; i < vec.size(); ++i)
        value.m_locs[i] = vec[i];
    return value;
}