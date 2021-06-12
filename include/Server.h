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
    virtual void updateLoc(const sf::Vector2f&, int);
    bool run(sf::RenderWindow& window);


private:
    //state
    bool m_launched;
    bool m_requiting;
    void registerPlayer();
    void updatePlayerState(const MemberInfo& member);
    void updateAboutNewMember(const AddMember&);
};