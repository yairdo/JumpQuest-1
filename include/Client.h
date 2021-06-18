#pragma once
#include <SFML/Network.hpp>
#include <unordered_set>
#include <string>
#include <NetworkObject.h>
#include <MessegesStructs.h>
#include <Macros.h>


class Client : public NetworkObject{
public:
	Client();
	virtual ~Client();

	bool handleRequests(int = 10);
	virtual bool launch();
	virtual void updateLoc(const MemberInfo&);
	virtual void sendStaticCollision(int);
private:
	sf::IpAddress m_serverIP;
	std::unordered_set<std::string> m_servers;
	bool m_isLinked;

	virtual void notifyClosing();
	void searchForServers();
	void regesterServer();
	virtual void setName(const char name[PLAYER_NAME_LEN], int index = -1);
	void updateMovingObj();
	void sendGameMembership(const char name[]);
};