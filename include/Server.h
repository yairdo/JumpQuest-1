#pragma once
#include <unordered_map>
#include <memory>
#include <SFML/Network.hpp>
#include <NetworkObject.h>
#include <MessegesStructs.h>


class Server : public NetworkObject{
public:
    Server();
    virtual ~Server() = default;
    bool launch();
    virtual bool handleRequests(int = 1)override;
    virtual void notifyClosing()override;
    virtual void updateLoc(const MemberInfo&);
    virtual void setName(const char name[PLAYER_NAME_LEN], int index = -1);
    virtual void sendStaticCollision(int);
    //test
    void sendNewInfo(const std::vector<MovingObjInfo>& vec);

    void startGame();

private:
    //state
    bool m_launched;
    bool m_requiting;
    std::vector<std::unique_ptr<sf::TcpSocket>> m_tcpSockets;

    void registerPlayer();
    void updatePlayerState(const MemberInfo& member);
    void updateStaticObjState(const StaticObjInfo& info);
    void updateAboutNewMember(const AddMember&);
    int countServersInPort();
    bool renameMember();
};