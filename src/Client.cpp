#include <Client.h>
#include <iostream>

Client::Client() : NetworkObject(), m_serverIP(), m_servers(), 
m_isLinked(false), m_started(false){
	m_packet.clear();
}
//============================================================================
bool Client::run(sf::RenderWindow&) {
	system("CLS");
	while (!m_started) {
		if (handleRequests()) {
			system("CLS");
			for (auto servers : m_servers)
				std::cout << "The server IP is: " << servers << std::endl;
			std::cout << "you are: " << getInfo().m_name << "| your id is: " 
				<< getInfo().m_id << std::endl;
			for (int i = 0; i < MAX_SERVER_PLAYERS; ++i) {
				if (getMembers(i)) {
					std::cout << getMembers(i)->m_id << ". " << getMembers(i)->m_name << std::endl;
				}
			}
		}
		else
			if(!m_isLinked)
			searchForServers();
		if (m_servers.size() > 0 && getInfo().m_id == 0) {
			std::cout << "plaese enter your nickname: ";
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
				char name[PLAYER_NAME_LEN];
				std::cin.getline(name, PLAYER_NAME_LEN);
				for (int i = 0; i < PLAYER_NAME_LEN; ++i)
					if (name[i] == '\n') {
						name[i] = '\0';
						break;
					}
				setName(name);
				sendGameMembership(name);
			}
		}
	}
	return(true);
}
/*============================================================================
* The method is receiving all the messeges the client received and handle them
* as needed.
*/
bool Client::handleRequests(int max) {
	int counter = 0;
	/*while (receivedTcpMessege() && counter++ < max) {
		std::cout << "tcp messege received.\n";
		try {
			switch (receiveTcpValue<Messege_type>())
			{
			case networkMessege:
				switch (receiveUdpValue<Network_messeges>()) {
					break;
				case startGame:
					m_started = true;
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
	}*/
	while (receivedUdpMessege()&& counter++ < max) {
		std::cout << "udp messege received.\n";
		try {
			switch (receiveUdpValue<Messege_type>())
			{
			case networkMessege:
				switch (receiveUdpValue<Network_messeges>()){
				case iAmFree:
					addServerToList();
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
void Client::updateLoc(const sf::Vector2f& loc, int state){
	sendUdpMessege<MemberInfo>(memberInfo, memberInfoCreator(getInfo().m_id, loc, state));
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

