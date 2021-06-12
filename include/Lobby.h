#pragma once
#include <SFML/Graphics.hpp>

class NetworkObject;

class Lobby {
public:
	Lobby();
	virtual ~Lobby() = default;


private:
	bool m_isServer;
	NetworkObject* m_networkObj;
};
