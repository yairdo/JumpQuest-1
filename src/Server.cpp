#include <Server.h>
#include <exception>
#include <iostream>
#include "Macros.h"
#include <Board.h>

Server::Server() :NetworkObject(SERVERS_PORT), m_requiting(false),
	m_launched(false){
}

Server::~Server() {
	notifyClosing();
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
		gameMemberCreator(getIP(), getPort(), "", memberInfoCreator())));
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
	while (receivedMessege() && counter++ < max) {
			Messege_type type = receiveValue<Messege_type>();
			if (getIP() != getSenderIP() || getPort() != getSenderPort()) {
				switch (type)
				{
				case networkMessege:
					switch (receiveValue<Network_messeges>()) {
					case whoIsAServer:
						if (m_launched)
							sendMessege(networkMessege, iAmAServer,
								sf::IpAddress::Broadcast, SERVERS_PORT);
						break;
					case whoIsFreeServer:
						if (m_requiting && m_launched)
							sendMessege(networkMessege, iAmFree);
						break;
					case closing:
						notifyCloser();
					}
					break;
				case memberInfo:
					updateLoc(receiveValue<MemberInfo>());
					break;
				case singMeIn:
					registerPlayer();
					break;
				case staticObjInfo:
					updateStaticObjState(receiveValue<StaticObjInfo>());
				default:
					break;
				}
			}
		}
	if (counter > 0)
		return true;
	return false;
}
/*============================================================================
* 
*/
void Server::notifyCloser(){
	for (int i = 1; i < MAX_SERVER_PLAYERS; ++i) {
		if (getMembers(i) && getSenderIP() == getMembers(i)->m_memberIp 
			&& getSenderPort() == getMembers(i)->m_memberPort) {
			for (int j = 1; j < MAX_SERVER_PLAYERS; ++j)
				if (getMembers(j) && i != j)
					sendMessege(closer, i, getMembers(j)->m_memberIp, getMembers(j)->m_memberPort);
		}
		setMember(i, nullptr);
	}
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
					receiveValue<GameMember>().m_name, memberInfoCreator(i))));
			//tell the new member his id
			sendMessege<int>(memberId, i);
			//notify old members about the new member
			updateAboutNewMember(
				addMemberCreator(getMembers(i)->m_info.m_id, getMembers(i)->m_name));
			//send the new mebemer all the old members info.
			for (int j = 0; j < MAX_SERVER_PLAYERS; ++j)
				if (i != j && getMembers(j)) {
					sendMessege<AddMember>(addMember,
						addMemberCreator(getMembers(j)->m_info.m_id, getMembers(j)->m_name),
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
		if(getMembers(i))
			sendMessege<Network_messeges>(networkMessege, closing,
				getMembers(i)->m_memberIp, getMembers(i)->m_memberPort);
	m_launched = false;
	m_requiting = false;
}
/*============================================================================
* the
*/
void Server::updateLoc(const MemberInfo& member) {
	updateMember(member);
	for (int i = 1; i < MAX_SERVER_PLAYERS; ++i) {
		if (getMembers(i) && i != member.m_id) {
			sendMessege<MemberInfo>(memberInfo, member,
				getMembers(i)->m_memberIp, getMembers(i)->m_memberPort);
		}
	}
}
//============================================================================
void Server::sendStaticCollision(int index) {
	updateStaticObjState(staticObjInfoCreator(getInfo().m_info.m_id, index));
}
/*============================================================================
* The method is notify the other players when another player collided with something
*/
void Server::updateStaticObjState(const StaticObjInfo& info) {
	if(info.m_id != getInfo().m_info.m_id)
		getBoard()->updateStaticMsgCollision(info.m_index);
	for (int i = 1; i < MAX_SERVER_PLAYERS; ++i) {
		if (getMembers(i) && i != info.m_id) {
			sendMessege<StaticObjInfo>(staticObjInfo, info,
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
				sendMessege<AddMember>(addMember, newMember,
					getMembers(i)->m_memberIp, getMembers(i)->m_memberPort);
}
/*==========================================================================*/
int Server::countServersInPort() {
	int counter = 0,
		max = 200,
		messegesCounter = 0;
	try {
		sendMessege(networkMessege, whoIsAServer,
			sf::IpAddress::Broadcast, SERVERS_PORT);
		while (receivedMessege() && messegesCounter++ < max) {
			if (receiveValue<Messege_type>() == networkMessege
				&& receiveValue<Network_messeges>() == iAmAServer)
				++counter;
		}
	}
	catch (std::exception& e) {
		std::cout << e.what();
	}
	return counter;
}
/*============================================================================*/
bool Server::renameMember() {
	for (int i = 0; i < MAX_SERVER_PLAYERS; ++i)
		//checks if the sender is already member.
		if (getMembers(i))
			if (getMembers(i)->m_memberIp == getSenderIP()
				&& getMembers(i)->m_memberPort == getSenderPort()) {
				updateAboutNewMember(addMemberCreator(i,
					receiveValue<GameMember>().m_name));
				sendMessege<int>(memberId, getMembers(i)->m_info.m_id,
					getSenderIP(), getSenderPort());
				return true;
			}
	return false;
}
/*==========================================================================*/
void Server::setName(const char name[PLAYER_NAME_LEN], int index) {
	updateAboutNewMember(addMemberCreator(
		(index == -1) ? getInfo().m_info.m_id : index, name));
}
/*==========================================================================*/
void Server::sendNewInfo(const std::vector<MovingObjInfo>& vec) {
	for (int i = 1; i < MAX_SERVER_PLAYERS; ++i)
		if (getMembers(i))
			sendMessege<MovingObjMembersRoport>(movingObj, testLocsCreator(vec),
				getMembers(i)->m_memberIp, getMembers(i)->m_memberPort);
}
/*==========================================================================*/
void Server::startGame() {
	m_requiting = false;
	for (int i = 1; i < MAX_SERVER_PLAYERS; ++i)
		if (getMembers(i))
			sendMessege(networkMessege, Network_messeges::startGame, getMembers(i)->m_memberIp,
				getMembers(i)->m_memberPort);
}