#include <Server.h>
#include <exception>
#include <iostream>
#include "Macros.h"
#include <Board.h>

Server::Server() :NetworkObject(SERVERS_PORT), m_requiting(false),
	m_launched(false){
}
/*==========================================================================*/
Server::~Server() {
	notifyClosing();
	m_launched = false;
	m_requiting = false;
}
/*============================================================================
* Launch the server
* The method check if the server launch is possible,
* if it is the launch bool is true.
*/
bool Server::launch() {
	if (m_launched)
		return true;
	if (!socketLaunched())
		bindSocket(SERVERS_PORT);
	if (countServersInPort() >= MAX_SERVERS_NUM || !socketLaunched())
		return false;

	setMember(0, std::make_unique<GameMember>(
		gameMemberCreator(getIP(), getPort(), "", memberInfoCreator())));
	setId(0);
	m_launched = true;
	m_requiting = true;
	return true;
}
/*============================================================================
* The method receive and respond to all of the received messages.
*/
bool Server::handleRequests(int max) {
	int counter = 0;
	while (receivedMessage() && counter++ < max) {
			MessageType type = receiveValue<MessageType>();
			if (getIP() != getSenderIP() || getPort() != getSenderPort()) {
				switch (type){
				case networkMessage:
					handleNetworkMessage();
					break;
				case memberInfo:
					updateLoc(receiveValue<MemberInfo>());
					break;
				case singMeIn:
					registerPlayer();
					break;
				case staticObjInfo:
					updateStaticObjState(receiveValue<StaticObjInfo>());
					break;
				case closer:
					notifyCloser(receiveValue<int>());
					break;
				case MessageType::addProjectile:
					addProjectile(receiveValue<AddProjectileMessage>());
					break;
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
* The
*/
void Server::notifyCloser(int index){
	setMember(index, nullptr);
	for (int i = 1; i < MAX_SERVER_PLAYERS; ++i)
		if (getMember(i))
			sendMessage<int>(closer, index, getMember(i)->m_memberIp, getMember(i)->m_memberPort);
}
/*============================================================================
* The method add the last message sender to the player list.
*/
void Server::registerPlayer() {
	if (renameMember())
		return;
	for (int i = 1; i < MAX_SERVER_PLAYERS; ++i) {
		if (!getMember(i)) {
			//add member to the server's member list
			setMember(i, std::make_unique<GameMember>(
				gameMemberCreator(getSenderIP(), getSenderPort(),
					receiveValue<GameMember>().m_name, memberInfoCreator(i))));
			//tell the new member his id
			sendMessage<int>(memberId, i);
			//notify old members about the new member
			updateAboutNewMember(
				addMemberCreator(getMember(i)->m_info.m_id, getMember(i)->m_name));
			//send the new mebemer all the old members info.
			for (int j = 0; j < MAX_SERVER_PLAYERS; ++j)
				if (i != j && getMember(j)) {
					sendMessage<AddMember>(addMember,
						addMemberCreator(getMember(j)->m_info.m_id, getMember(j)->m_name),
						getMember(i)->m_memberIp, getMember(i)->m_memberPort);
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
		if(getMember(i))
			sendMessage<NetworkMessages>(networkMessage, closing,
				getMember(i)->m_memberIp, getMember(i)->m_memberPort);
}
/*============================================================================
* The method is update the received member location on the map. 
*/
void Server::updateLoc(const MemberInfo& member) {
	updateMember(member);
	for (int i = 1; i < MAX_SERVER_PLAYERS; ++i) {
		if (getMember(i) && i != member.m_id) {
			sendMessage<MemberInfo>(memberInfo, member,
				getMember(i)->m_memberIp, getMember(i)->m_memberPort);
		}
	}
}
/*==========================================================================*/
void Server::sendStaticCollision(int index) {
	updateStaticObjState(staticObjInfoCreator(getInfo().m_info.m_id, index));
}
/*============================================================================
* The method is notify the other players when another player collided with 
* something
*/
void Server::updateStaticObjState(const StaticObjInfo& info) {
	if(info.m_id != getInfo().m_info.m_id)
		getBoard()->updateStaticMsgCollision(info.m_index);
	for (int i = 1; i < MAX_SERVER_PLAYERS; ++i) {
		if (getMember(i) && i != info.m_id) {
			sendMessage<StaticObjInfo>(staticObjInfo, info,
				getMember(i)->m_memberIp, getMember(i)->m_memberPort);
		}
	}
}
/*============================================================================
* The method is notify the other players about the new registered member.
*/
void Server::updateAboutNewMember(const AddMember& newMember) {
	NetworkObject::setName(newMember.m_name, newMember.m_id);
	for (int i = 1; i < MAX_SERVER_PLAYERS; ++i)
		if (getMember(i))
			if (i != newMember.m_id)
				sendMessage<AddMember>(addMember, newMember,
					getMember(i)->m_memberIp, getMember(i)->m_memberPort);
}
/*==========================================================================
* The method 
*/
int Server::countServersInPort() {
	int counter = 0,
		max = 200,
		messagesCounter = 0;
	try {
		sendMessage(networkMessage, whoIsAServer,
			sf::IpAddress::Broadcast, SERVERS_PORT);
		while (receivedMessage(0.1) && messagesCounter++ < max) {
			if (receiveValue<MessageType>() == networkMessage
				&& receiveValue<NetworkMessages>() == iAmAServer)
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
		if (getMember(i))
			if (getMember(i)->m_memberIp == getSenderIP()
				&& getMember(i)->m_memberPort == getSenderPort()) {
				updateAboutNewMember(addMemberCreator(i,
					receiveValue<GameMember>().m_name));
				sendMessage<int>(memberId, getMember(i)->m_info.m_id,
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
		if (getMember(i))
			sendMessage<MovingObjMembersRoport>(movingObj, testLocsCreator(vec),
				getMember(i)->m_memberIp, getMember(i)->m_memberPort);
}
/*==========================================================================*/
void Server::startGame(MapType lvl) {
	for (int i = 1; i < MAX_SERVER_PLAYERS; ++i)
		if (getMember(i))
			sendMessage<MapType>(MessageType::startGame, lvl, getMember(i)->m_memberIp,
				getMember(i)->m_memberPort);
}
/*============================================================================
* 
*/
void Server::handleNetworkMessage() {
	switch (receiveValue<NetworkMessages>()) {
	case whoIsAServer:
		if (m_launched)
			sendMessage(networkMessage, iAmAServer,
				getSenderIP(), getSenderPort());
		break;
	case whoIsFreeServer:
		if (m_requiting && m_launched)
			sendMessage(networkMessage, iAmFree);
		break;
	default:
		break;
	}
}
/*============================================================================
*/
void Server::addProjectile(const AddProjectileMessage& projectile){
	 getBoard()->addProjectile(projectile);
	 for (int i = 1; i < MAX_SERVER_PLAYERS; ++i)
		 if (getMember(i))
			 sendMessage<AddProjectileMessage>(MessageType::addProjectile, projectile, 
				getMember(i)->m_memberIp, getMember(i)->m_memberPort);
 }