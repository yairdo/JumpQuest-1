#include <Client.h>
#include <iostream>
#include <Board.h>

Client::Client() : NetworkObject(), m_serverIP(), m_servers(), 
m_isLinked(false), m_started(false){
	m_packet.clear();
}
/*============================================================================
* The method is receiving all the messeges the client received and handle them
* as needed.
*/
bool Client::handleRequests(int max) {
	int counter = 0;
	TestLocs info;
	while (receivedUdpMessege()&& counter++ < max) {
		std::cout << "udp messege received.\n";
		try {
			switch (receiveUdpValue<Messege_type>())
			{
			case networkMessege:
				switch (receiveUdpValue<Network_messeges>()){
				case iAmFree:
					sendGameMembership("client");
					break;
				case startGame:
					setStarted(true);
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
				info = receiveUdpValue<TestLocs>();
				for (int i = 0; i < info.m_size; ++i)
					getBoard()->setInfo(i, info.m_locs[i]);
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
* The method add the last messege sender to the avaible servers list.
*/
void Client::addServerToList() {
	if (m_servers.find(getSenderIP().toString()) == m_servers.end())
		m_servers.insert(getSenderIP().toString());
}
/*============================================================================
* The method notify the host Server that the client is disconnecting.
*/
void Client::notifyClosing() {
	sendUdpMessege(networkMessege, closing, m_serverIP, SERVERS_PORT);
}
/*==========================================================================*/
void Client::updateLoc( const MemberInfo& member){
	sendUdpMessege<MemberInfo>(memberInfo, member);
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
* The method is singIn to the server.
*/
void Client::sendGameMembership(const char name[]){
	sendUdpMessege<GameMember>(singMeIn, gameMemberCreator(getIP(), getPort(), name));
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
* The method sets a new name for the client
*/
void Client::setName(const char name[PLAYER_NAME_LEN], int index) {
	NetworkObject::setName(name);
	sendUdpMessege<GameMember>(singMeIn, getInfo());
}
