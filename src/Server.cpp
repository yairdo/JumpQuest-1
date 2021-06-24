#include <Server.h>
#include <exception>
#include <iostream>
#include "Macros.h"
#include <Board.h>

Server::Server() :NetworkObject(SERVERS_PORT), m_requiting(false),
	m_launched(false), m_PlayersReady(MAX_SERVER_PLAYERS, false){}
/*==========================================================================*/
Server::~Server() {
	notifyClosing();
	m_launched = false;
	m_requiting = false;
}
/*============================================================================
* Launch the server
* The method check if the server launching is possible,
* if it is, the method return true, otherwise the method return false.
*/
bool Server::launch() {
	if (m_launched)
		return true;
	if (!socketLaunched())
		bindSocket(SERVERS_PORT);
	if (portInUse() )
		throw(std::exception("Too many servers are opened."));
	if (!socketLaunched())
		throw(std::exception("Bind socket failure! please try again."));

	setMember(0, std::make_unique<GameMember>(
		GameMember(getIP(), getPort(), "", MemberInfo())));
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
	MovingObjInfo movingInfo;
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
				case movingObjInfo:
					movingInfo = receiveValue<MovingObjInfo>();
					getBoard()->setInfo(movingInfo.m_index, movingInfo);
					break;
				case closer:
					notifyCloser(receiveValue<int>());
					break;
				case MessageType::addProjectile:
					addProjectile(receiveValue<AddProjectileMessage>());
					break;
				case notifyWin:
					notifyWinning(receiveValue<unsigned short>());
					break;
				case iAmReady:
					m_PlayersReady[receiveValue<int>()] = true;
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
* The method notify the regestered members about closing member, and sing him
* out.
*/
void Server::notifyCloser(int index){
	setMember(index, nullptr);
	for (int i = 1; i < MAX_SERVER_PLAYERS; ++i)
		if (getMember(i))
			sendMessage<int>(closer, index, getMember(i)->m_memberIp, 
				getMember(i)->m_memberPort);
}
/*============================================================================
* The method add the last message sender to the player list.
*/
void Server::registerPlayer() {
	if (!m_requiting || renameMember())
		return;
	for (int i = 1; i < MAX_SERVER_PLAYERS; ++i)
		if (!getMember(i))
			addNewMember(i);
}
/*============================================================================
*/
void Server::addNewMember(int index) {
	//add member to the server's member list
	setMember(index, std::make_unique<GameMember>(GameMember(getSenderIP(), 
		getSenderPort(), receiveValue<GameMember>().m_name, MemberInfo(index))));
	//tell the new member his id
	sendMessage<int>(memberId, index);
	//notify old members about the new member
	updateAboutNewMember(AddMember(getMember(index)->m_info.m_id, 
		getMember(index)->m_name));
	//send the new mebemer all the old members info.
	for (int j = 0; j < MAX_SERVER_PLAYERS; ++j)
		if (index != j && getMember(j)) {
			sendMessage<AddMember>(addMember,
				AddMember(getMember(j)->m_info.m_id, getMember(j)->m_name),
				getMember(index)->m_memberIp, getMember(index)->m_memberPort);
		}
	return;
}
/*============================================================================
* The method notify all of the server's clients that the server is closing
* itself.
*/
void Server::notifyClosing() {
	for (int i = 1; i < MAX_SERVER_PLAYERS; ++i)
		if(getMember(i))
			sendMessage<NetworkMessages>(networkMessage, closing,
				getMember(i)->m_memberIp, getMember(i)->m_memberPort, true);
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
	updateStaticObjState(StaticObjInfo(getInfo().m_info.m_id, index));
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
					getMember(i)->m_memberIp, getMember(i)->m_memberPort, true);
}
/*==========================================================================
* The method is checking if there is no server in the SERVERS_PORT.
* If the port is free, the method will return true, otherwise, the method
* will return false.
*/
bool Server::portInUse() {
	int max = 50,
		messagesCounter = 0;
	sendMessage<NetworkMessages>(networkMessage, whoIsAServer,
		sf::IpAddress::Broadcast, SERVERS_PORT, true);
	while (receivedMessage(0.5) && messagesCounter++ < max) {
		if (receiveValue<MessageType>() == networkMessage
			&& receiveValue<NetworkMessages>() == iAmAServer
			&& getSenderIP() != getIP())
			return true;
	}
	return false;
}
/*============================================================================
* The method is changing the member's info and notify the other clients about
* the new member name.
*/
bool Server::renameMember() {
	for (int i = 0; i < MAX_SERVER_PLAYERS; ++i)
		//checks if the sender is already member.
		if (getMember(i))
			if (getMember(i)->m_memberIp == getSenderIP()
				&& getMember(i)->m_memberPort == getSenderPort()) {
				updateAboutNewMember(AddMember(i, receiveValue<GameMember>().m_name));
				sendMessage<int>(memberId, getMember(i)->m_info.m_id, getSenderIP(),
					getSenderPort(), true);
				return true;
			}
	return false;
}
/*==========================================================================*/
void Server::setName(const char name[PLAYER_NAME_LEN], int index) {
	updateAboutNewMember(AddMember(
		(index == -1) ? getInfo().m_info.m_id : index, name));
}
/*==========================================================================
* the method update the clients about the new member
*/
void Server::sendNewInfo(const std::vector<MovingObjInfo>& vec) {
	for (int i = 1; i < MAX_SERVER_PLAYERS; ++i)
		if (getMember(i))
			sendMessage<MovingObjMembersRoport>(movingObj, MovingObjMembersRoport(vec),
				getMember(i)->m_memberIp, getMember(i)->m_memberPort);
}
/*==========================================================================*/
// notfies all players to start the game
void Server::startGame(MapType lvl) {
	m_requiting = false;
	setLvlInfo(lvl);
	for (int i = 1; i < MAX_SERVER_PLAYERS; ++i)
		if (getMember(i))
			sendMessage<MapType>(MessageType::startGame, lvl, getMember(i)->m_memberIp,
				getMember(i)->m_memberPort, true);
}
/*============================================================================
 * notifies all players that of the winner */
void Server::notifyWinning(unsigned short winner){
	if (winner == MAX_SERVER_PLAYERS)
		winner = 0;
	setWinner(winner);
	for (int i = 1; i < MAX_SERVER_PLAYERS; ++i)
		if (getMember(i))
			sendMessage<unsigned short>(notifyWin, winner, getMember(i)->m_memberIp, 
				getMember(i)->m_memberPort, true);
}
/*============================================================================
* If received NetworkMessage, the method is handling the message as needed.
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
			sendMessage<NetworkMessages>(networkMessage, iAmFree);
		break;
	case iAmAServer:
		throw(std::exception("Too many servers are opened."));
	default:
		break;
	}
}
/*==========================================================================
 * notifies all players to add a projectile to thier board*/

void Server::addProjectile(const AddProjectileMessage& projectile){
	 getBoard()->addProjectile(projectile);
	 for (int i = 1; i < MAX_SERVER_PLAYERS; ++i)
		 if (getMember(i))
			 sendMessage<AddProjectileMessage>(MessageType::addProjectile, projectile, 
				getMember(i)->m_memberIp, getMember(i)->m_memberPort, true);
 }
/*==========================================================================
 * syncs the starting time of the game with all players returns true when  
 * all players ready are */
bool Server::gameStarted() {
	for (int i = 1; i < MAX_SERVER_PLAYERS; ++i)
		if (getMember(i) && !m_PlayersReady[i])
			return false;
	for (int i = 1; i < MAX_SERVER_PLAYERS; ++i)
		if (getMember(i))
		sendMessage<int>(networkMessage, NetworkMessages::startGameMessage, 
			getMember(i)->m_memberIp, getMember(i)->m_memberPort);
	return true;
}