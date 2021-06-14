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
	if (countServersInPort() == MAX_SERVERS_NUM)
		return false;
	
	setMember(0, std::make_unique<GameMember>(
		gameMemberCreator(getIP(), getPort(), "", 0, 0)));
	setId(0);
	m_launched = true;
	m_requiting = true;
	return true;
}
/*============================================================================
* The method receive and respond to all of the received messeges.
*/
bool Server::handleRequests(int max) {
	int counter = 0;
	while (receivedUdpMessege() && counter++ < max) {
		try {
			Messege_type type = receiveUdpValue<Messege_type>();
			if (getIP() != getSenderIP() || getPort() != getSenderPort()) {
				switch (type)
				{
				case networkMessege:
					switch (receiveUdpValue<Network_messeges>()) {
					case whoIsAServer:
						if (m_launched)
							sendUdpMessege(networkMessege, iAmAServer, 
								sf::IpAddress::Broadcast, SERVERS_PORT);
						break;
					case whoIsFreeServer:
						if (m_requiting && m_launched) 
							sendUdpMessege(networkMessege, iAmFree);
						break;
					}
					break;
				case memberInfo:
					updatePlayerState(receiveUdpValue<MemberInfo>());
					break;
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
	if (renameMember())
		return;
	for (int i = 1; i < MAX_SERVER_PLAYERS; ++i) {
		if (!getMembers(i)) {
			//add member to the server's member list
			setMember(i, std::make_unique<GameMember>(
				gameMemberCreator(getSenderIP(), getSenderPort(),
					receiveUdpValue<GameMember>().m_name, i)));
			//tell the new member his id
			sendUdpMessege<int>(memberId, i);
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
	updatePlayerState(memberInfoCreator(0, loc, state));
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
			if (i != newMember.m_id)
				sendUdpMessege<AddMember>(addMember, newMember,
					getMembers(i)->m_memberIp, getMembers(i)->m_memberPort);
}
/*==========================================================================*/
int Server::countServersInPort(){
	int counter = 0, 
		max = 200, 
		messegesCounter = 0;
	try {
		sendUdpMessege(networkMessege, whoIsAServer,
			sf::IpAddress::Broadcast, SERVERS_PORT);
		while (receivedUdpMessege(0.1) && messegesCounter++ < max) {
			if (receiveUdpValue<Messege_type>() == networkMessege
				&& receiveUdpValue<Network_messeges>() == iAmAServer)
				++counter;
		}
	}
	catch (std::exception& e) {
		std::cout << e.what();
	}
	return counter;
}
/*============================================================================*/
bool Server::renameMember(){
	for (int i = 0; i < MAX_SERVER_PLAYERS; ++i)
		//checks if the sender is already member.
		if (getMembers(i))
			if (getMembers(i)->m_memberIp == getSenderIP() 
				&& getMembers(i)->m_memberPort == getSenderPort()) {
				updateAboutNewMember(addMemberCreator(i, 
					receiveUdpValue<GameMember>().m_name));
				sendUdpMessege<int>(memberId, getMembers(i)->m_id, 
					getSenderIP(), getSenderPort());
				return true;
			}
	return false;
}
/*==========================================================================*/
void Server::setName(const char name[PLAYER_NAME_LEN], int index) {
	updateAboutNewMember(addMemberCreator((index == -1)?getInfo().m_id:index, name));
}
/*==========================================================================*/
void Server::sendNewLoc(const sf::Vector2f& loc, int index){
	sendUdpMessege<MemberInfo>(movingObj, memberInfoCreator(index, loc));
}
/*==========================================================================*/
void Server::startGame() {
	for (int i = 1; i < MAX_SERVER_PLAYERS; ++i)
		if (getMembers(i))
			sendUdpMessege(networkMessege, Network_messeges::startGame, getMembers(i)->m_memberIp,
				getMembers(i)->m_memberPort);
}