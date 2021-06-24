#include <MessagesStructs.h>
#include <memory>

//all the Messages Structs c-tors

//============================================================================
GameMember::GameMember(const sf::IpAddress& ip, unsigned short port, 
    const char name[PLAYER_NAME_LEN], const MemberInfo& member ) {
    m_memberIp = ip;
    m_memberPort = port;
    m_info = member;
    std::memcpy(m_name, name, PLAYER_NAME_LEN);
}
//============================================================================
MemberInfo::MemberInfo(unsigned short id, const sf::Vector2f & loc, int row,
    int col, int dir,float time,bool win) {
    m_id = id;
    m_loc = loc;
    m_row = row;
    m_col = col;
    m_direction = dir;
    m_totalTime = time;
    m_win = win;
}
//============================================================================
AddMember::AddMember(unsigned short id, const char name[PLAYER_NAME_LEN]) {
    m_id = id;
    std::memcpy(m_name, name, PLAYER_NAME_LEN);
}
//============================================================================
MovingObjInfo::MovingObjInfo(const sf::Vector2f& loc, float time, 
    const b2Vec2& vel,bool active, unsigned short index){
    m_loc = loc;
    m_timer = time;
    m_vel = vel;
    m_active = active;
    m_index = index;
}
//============================================================================
MovingObjMembersRoport::MovingObjMembersRoport(const std::vector<MovingObjInfo>& vec) {
    m_size = vec.size();
    for (int i = 0; i < vec.size(); ++i)
        m_locs[i] = vec[i];
}
//============================================================================
StaticObjInfo::StaticObjInfo(unsigned short id,int index){
    m_id = id;
    m_index = index;
}
//============================================================================
AddProjectileMessage::AddProjectileMessage(const sf::Vector2f& from,
    const sf::Vector2f& to, const sf::Vector2f& bounds) {
    m_frome = from;
    m_to = to;
    m_bounds = bounds;
}
//============================================================================
StartMessage::StartMessage(int theme, int level) {
    m_theme = theme;
    m_level = level;
}