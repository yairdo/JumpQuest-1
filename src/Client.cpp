#include <Client.h>
#include <iostream>
#include <Board.h>

Client::Client() : NetworkObject(), m_serverIP(), m_servers(), 
m_isLinked(false), m_started(false){
	m_packet.clear();
}
//============================================================================
Client::~Client() {
	notifyClosing();
}

/*============================================================================
* The method is receiving all the messeges the client received and handle them
* as needed.
*/
bool Client::handleRequests(int max) {
	int counter = 0;
	while (receivedUdpMessege()&& counter++ < max) {
		//std::cout << "udp messege received.\n";
		try {
			switch (receiveUdpValue<Messege_type>())
			{
			case networkMessege:
				switch (receiveUdpValue<Network_messeges>()){
				case iAmFree:
					setName("client");
					break;
				case startGame:
					setStarted(true);
					break;
				case closing:
					throw std::exception("disconnected from server.\n");
					break;
				default:
					break;
				}
				break;
			case memberId:
				regesterServer();
				break;
			case addMember:
				addMemberToList();
				break;
			case memberInfo:
				updateMember(receiveUdpValue<MemberInfo>());
				break;
			case movingObj:
				updateMovingObj();
				break;
			case staticObjInfo:
				getBoard()->updateStaticMsgCollision(receiveUdpValue<StaticObjInfo>().m_index);
				break;
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
					exit(EXIT_FAILURE);
				}
			}
		}
	}
	if (counter == 0)
		return false;
	return true;
}
/*============================================================================
* The method send to all the avaible servers message informing them that
* the Client is searchig for hosting Server.
* to receive the servers answer run handleRequests method.
*/
void Client::searchForServers() {
	m_packet.clear();
	sendUdpMessege(networkMessege, whoIsFreeServer, 
		sf::IpAddress::Broadcast, SERVERS_PORT);
	m_packet.clear();
}
/*============================================================================
* The method notify the host Server that the client is disconnecting.
*/
void Client::notifyClosing() {
	sendUdpMessege(networkMessege, closing, m_serverIP, SERVERS_PORT);
}
/*==========================================================================*/
void Client::updateLoc( const MemberInfo& member){
	sendUdpMessege<MemberInfo>(memberInfo, member, m_serverIP, SERVERS_PORT);
}
/*==========================================================================*/
bool Client::launch()
{
	if (!receivedUdpMessege()){
		searchForServers();
	}
	return getInfo().m_info.m_id != 0;
}
/*============================================================================
* The method is regester to the server the client reseived messege from the last.
*/
void Client::regesterServer() {
	setId(receiveUdpValue<int>());
	m_serverIP = getSenderIP();
	m_isLinked = true;
}
/*============================================================================
* The method sets a new name for the client and singIn to the server.
*/
void Client::setName(const char name[PLAYER_NAME_LEN], int index) {
	NetworkObject::setName(name);
	sendUdpMessege<GameMember>(singMeIn, getInfo());
}
/*============================================================================
* The method is notify the server about collision with static obj
*/
void Client::sendStaticCollision(int index){
	sendUdpMessege<StaticObjInfo>(staticObjInfo, 
		staticObjInfoCreator(getInfo().m_info.m_id, index));
}
/*============================================================================
* The method is update the Board's moving objects as the server reported.
*/
void Client::updateMovingObj() {
	MovingObjMembersRoport messege = receiveUdpValue<MovingObjMembersRoport>();
	for (int i = 0; i < messege.m_size; ++i)
		getBoard()->setInfo(i + 1, messege.m_locs[i]);
}