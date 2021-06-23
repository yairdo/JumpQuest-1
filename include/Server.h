#pragma once
#include <unordered_map>
#include <memory>
#include <SFML/Network.hpp>
#include <NetworkObject.h>
#include <MessagesStructs.h>


class Server : public NetworkObject{
public:
    Server();
    virtual ~Server();
    bool launch();
    virtual bool handleRequests(int = 1)override;
    virtual void updateLoc(const MemberInfo&)override;
    virtual void setName(const char name[PLAYER_NAME_LEN], int index = -1)override;
    virtual void sendStaticCollision(int)override;
    virtual void addProjectile(const AddProjectileMessage&)override;
    void startGame(MapType);
    virtual void notifyWinning(unsigned short) override;
    virtual bool gameStarted();

    //test
    void sendNewInfo(const std::vector<MovingObjInfo>& vec);

private:
    //state
    bool m_launched;
    bool m_requiting;
    std::vector<bool> m_PlayersReady;

    virtual void notifyClosing()override;
    void notifyCloser(int);
    void registerPlayer();
    void handleNetworkMessage();
    void updateStaticObjState(const StaticObjInfo& info);
    void updateSingleMovingObjInfo(const MovingObjInfo& info) {}
    void updateAboutNewMember(const AddMember&);
    int countServersInPort();
    bool renameMember();
    
};