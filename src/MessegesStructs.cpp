#include <MessegesStructs.h>
#include <memory>
//============================================================================
GameMember gameMemberCreator(const sf::IpAddress& ip, unsigned short port, const char name[PLAYER_NAME_LEN], const MemberInfo& member ) {
    GameMember value;
    value.m_memberIp = ip;
    value.m_memberPort = port;
    value.m_info = member;
    std::memcpy(value.m_name, name, PLAYER_NAME_LEN);
    return value;
}
//============================================================================
MemberInfo memberInfoCreator(unsigned short id, const sf::Vector2f & loc, int row, int col, int dir) {
    MemberInfo value;
    value.m_id = id;
    value.m_loc = loc;
    value.m_row = row;
    value.m_col = col;
    value.m_direction = dir;
    return value;
}
//============================================================================
AddMember addMemberCreator(unsigned short id, const char name[PLAYER_NAME_LEN]) {
    AddMember value;
    value.m_id = id;
    std::memcpy(value.m_name, name, PLAYER_NAME_LEN);
    return value;
}
//============================================================================
MovingObjInfo movingObjInfoCreator(const sf::Vector2f& loc, float time, const b2Vec2& vel){
    MovingObjInfo value;
    value.m_loc = loc;
    value.m_timer = time;
    value.m_vel = vel;
    return value;
}
//============================================================================
TestLocs testLocsCreator(const std::vector<MovingObjInfo>& vec) {
    TestLocs value;
    value.m_size = vec.size();
    for (int i = 0; i < vec.size(); ++i)
        value.m_locs[i] = vec[i];
    return value;
}
//============================================================================
StaticObjInfo staticObjInfoCreator(unsigned short id,int index){ 
    StaticObjInfo info;
    info.m_id = id;
    info.m_index = index;
    return info;
}