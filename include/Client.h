#pragma once
#include <SFML/Network.hpp>
#include <unordered_set>
#include <string>
#include <NetworkObject.h>
#include <MessegesStructs.h>


class Client : public 
	NetworkObject{
public:
	Client();
	virtual bool run(sf::RenderWindow&);
	virtual ~Client() = default;
	bool handleRequests(int = 10);
	void searchForServers();
	void regesterServer();
	virtual void notifyClosing();
	void sendGameMembership(const char name[20]);
	virtual void updateLoc(const sf::Vector2f&, int);
private:
	sf::IpAddress m_serverIP;
	std::unordered_set<std::string> m_servers;
	bool m_isLinked;
	bool m_started;

	void addServerToList();
}; 