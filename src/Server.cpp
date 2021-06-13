#include <Server.h>
#include <exception>
#include <iostream>
#include "Macros.h"
/*
* TODO
* inform members the server list.
*/
Server::Server() :NetworkObject(SERVERS_PORT), m_requiting(false),
m_launched(false), m_tcpSockets(MAX_SERVER_PLAYERS) {
}
/*============================================================================
* Launch the server
* The method check if the server launch is possible,
* if it is the launch bool is true.
*/
bool Server::launch() {
	if (m_launched)
		return true;
	try {
		sendUdpMessege(networkMessege, whoIsAServer,
			sf::IpAddress::Broadcast, SERVERS_PORT);
	}
	catch (std::exception& e) {
		std::cout << e.what();
		return false;
	}
	m_packet.clear();
	unsigned int counter = 0;
	while (receivedUdpMessege()) {
		try {
			if (receiveUdpValue<Messege_type>() == networkMessege
				&& receiveUdpValue<Network_messeges>() == iAmAServer)
				++counter;
		}
		catch (std::exception& e) {
			std::cout << e.what();
			//server isn't launched so no need to receive other messeges than network messeges type
			continue;
		}
		if (counter == MAX_SERVERS_NUM)
			return false;
	}
	setMember(0, std::make_unique<GameMember>(
		gameMemberCreator(getIP(), getPort(), "Host", 1, 0)));
	setId(1);
	m_launched = true;
	m_requiting = true;
	return true;
}
/*============================================================================
* The method receive and respond to all of the received messeges.
*/
bool Server::handleRequests(int max) {
	int counter = 0;
	/*while (receivedTcpMessege() && counter < max) {
		std::cout << "tcp messege received.\n";
		try {
			switch (receiveTcpValue<Messege_type>())
			{
			case singMeIn:
				registerPlayer();
			default:
				break;
			}
		}
		catch (std::exception& e) {
			if (e.what() == SOKET_ERROR) {
				try {
					notifyClosing();
					return false;
				}
				catch (std::exception& e2) {
					std::cout << e2.what();
					exit(EXIT_FAILURE);
				}
			}
		}
		++counter;
	}*/

	while (receivedUdpMessege() && counter++ < max) {
		try {
			std::cout << "udp messege received.\n";
			Messege_type type = receiveUdpValue<Messege_type>();
			if (getIP() != getSenderIP() || getPort() != getSenderPort()) {
				switch (type)
				{
				case networkMessege:
					switch (receiveUdpValue<Network_messeges>()) {
					case whoIsAServer:
						if (m_launched) {
							sf::TcpSocket tempSocket;
							tempSocket.connect(getSenderIP(), getSenderPort());
							sendTcpMessege(networkMessege, iAmAServer, tempSocket);
						}
						break;
					case whoIsFreeServer:
						if (m_requiting && m_launched) {
							sendUdpMessege(networkMessege, iAmFree);
						}
						break;
					}
					break;
				case memberInfo:
					updatePlayerState(receiveUdpValue<MemberInfo>());
					//send
				case singMeIn:
					registerPlayer();
					break;
				default:
					break;
				}
			}
		}
		catch (std::exception& e) {
			if (e.what() == SOKET_ERROR) {
				try {
					notifyClosing();
					return false;
				}
				catch (std::exception& e2) {
					std::cout << e2.what();
					exit(EXIT_FAILURE);
				}
			}
		}
	}
	if (counter > 0)
		return true;
	return false;
}
/*============================================================================
* The method add the last messege sender to the player list.
*/
void Server::registerPlayer() {
	for (int i = 0; i < MAX_SERVER_PLAYERS; ++i)
		//checks if the sender is already member.
		if (getMembers(i)) {
			if (getMembers(i)->m_memberIp == getSenderIP() && getMembers(i)->m_memberPort == getSenderPort()) {
				sendUdpMessege<int>(memberId, getMembers(i)->m_id, getSenderIP(), getSenderPort());
				updateAboutNewMember(receiveUdpValue<AddMember>());
				return;
			}
		}
	for (int i = 1; i < MAX_SERVER_PLAYERS; ++i) {
		if (!getMembers(i)) {
			//add member to the server's member list
			setMember(i, std::make_unique<GameMember>(
				gameMemberCreator(getSenderIP(), getSenderPort(),
					receiveUdpValue<GameMember>().m_name, i + 1)));
			//open socket to the new client.
			m_tcpSockets[i] = std::make_unique<sf::TcpSocket>();
			m_tcpSockets[i]->connect(getSenderIP(), getSenderPort());
			//tell the new member his id
			sendUdpMessege<int>(memberId, i + 1);
			//sendTcpMessege<int>(memberId, i + 1, *m_tcpSockets[i]);
			//notify old members about the new member
			updateAboutNewMember(
				addMemberCreator(getMembers(i)->m_id, getMembers(i)->m_name));
			//send the new mebemer all the old members info.
			for (int j = 0; j < MAX_SERVER_PLAYERS; ++j)
				if (i != j && getMembers(j)) {
					sendUdpMessege<AddMember>(addMember,
						addMemberCreator(getMembers(j)->m_id, getMembers(j)->m_name),
						getMembers(i)->m_memberIp, getMembers(i)->m_memberPort);
				}
			m_packet.clear();
			break;
		}
	}
}
/*============================================================================
* The method notify all of the server's clients that the server is closing
* itself.
*/
void Server::notifyClosing() {
	for (int i = 1; i < MAX_SERVER_PLAYERS; ++i)
		sendUdpMessege<Network_messeges>(networkMessege, closing,
			getMembers(i)->m_memberIp, getMembers(i)->m_memberPort);
	m_launched = false;
	m_requiting = false;
}
/*============================================================================
*/
void Server::updateLoc(const sf::Vector2f& loc, int state) {
	updatePlayerState(memberInfoCreator(1, loc, state));
}
/*============================================================================
* The method update the other players about the player new state update.
*/
void Server::updatePlayerState(const MemberInfo& member) {
	updateMember(member);
	for (int i = 1; i < MAX_SERVER_PLAYERS; ++i) {
		if (getMembers(i)) {
			sendUdpMessege<MemberInfo>(memberInfo, member,
				getMembers(i)->m_memberIp, getMembers(i)->m_memberPort);
		}
	}
}
/*============================================================================
* The method is notify the other players
*/
void Server::updateAboutNewMember(const AddMember& newMember) {
	NetworkObject::setName(newMember.m_name, newMember.m_id);
	for (int i = 1; i < MAX_SERVER_PLAYERS; ++i)
		if (getMembers(i))
			if (i + 1 != newMember.m_id)
				sendUdpMessege<AddMember>(addMember, newMember,
					getMembers(i)->m_memberIp, getMembers(i)->m_memberPort);
}
/*==========================================================================*/
void Server::setName(const char name[PLAYER_NAME_LEN], int index) {
	//NetworkObject::setName(name);
	updateAboutNewMember(addMemberCreator(index+1, name));
}
/*==========================================================================*/
void Server::startGame() {
	for (int i = 0; i < MAX_SERVER_PLAYERS; ++i)
		if (getMembers(i))
			sendUdpMessege(networkMessege, Network_messeges::startGame, getMembers(i)->m_memberIp,
				getMembers(i)->m_memberPort);
}