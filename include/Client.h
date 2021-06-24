#pragma once
#include <SFML/Network.hpp>
#include <unordered_set>
#include <string>
#include <NetworkObject.h>

class MessagesStructs;

class Client : public NetworkObject{
public:
	//------------c-tor-----------
	Client();

	//------------d-tor-----------
	virtual ~Client();

	//----------Functions---------
	bool handleRequests(int = 10);
	virtual bool launch();
	virtual void updateLoc(const MemberInfo&);
	virtual void sendStaticCollision(int);
	virtual void updateSingleMovingObjInfo(const MovingObjInfo& info) override;
	virtual void addProjectile(const AddProjectileMessage& projectile)override;
	virtual void notifyWinning(unsigned short) override;
	void sendImReady() ;

	virtual bool gameStarted() { return m_gameStarted; }

private:
	//---------Membmbers----------

	sf::IpAddress m_serverIP;
	std::unordered_set<std::string> m_servers;
	bool m_isLinked;
	bool m_gameStarted;

	//----------Functions---------

	virtual void notifyClosing();
	void searchForServers();
	void regesterServer();
	virtual void setName(const char name[PLAYER_NAME_LEN], int index = -1);
	void updateMovingObj();
	void sendGameMembership(const char name[]);
	void handleNetworkMessage();
};