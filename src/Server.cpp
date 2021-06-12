#include <Server.h>
#include <exception>
#include <iostream>

/*
* TODO
* inform members the server list.
*/
Server::Server() : NetworkObject(SERVERS_PORT), m_requiting(false),
	m_launched(false) {
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
		sendNetworkMessege(whoIsAServer, sf::IpAddress::Broadcast, SERVERS_PORT);
	}
	catch (std::exception& e) {
		std::cout << e.what();
		return false;
	}
	m_packet.clear();
	unsigned int counter = 0;
	while (receivedMessege()) {
		try {
			if (receiveValue<Messege_type>() == networkMessege
				&& receiveValue<Network_messeges>() == iAmAServer)
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
	while (receivedMessege() && counter++ < max) {
		std::cout << "received!\n";
		try {
			Messege_type type = receiveValue<Messege_type>();
			if (getIP() != getSenderIP() || getPort() != getSenderPort()) {
				switch (type)
				{
				case networkMessege:
					switch (receiveValue<Network_messeges>()) {
					case whoIsAServer:
						if (m_launched)
							sendNetworkMessege(iAmAServer);
						break;
					case whoIsFreeServer:
						if (m_requiting && m_launched)
							sendNetworkMessege(iAmFree);
						break;
					}
					break;
				case singMeIn:
					registerPlayer();
					break;
				case memberInfo:
					updatePlayerState(receiveValue<MemberInfo>());
					//send
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
	return true;
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
				return;
			}
		}
	for (int i = 1; i < MAX_SERVER_PLAYERS; ++i) {
		if (!getMembers(i)) {
			//add member to the server's member list
			setMember(i, std::make_unique<GameMember>(
				gameMemberCreator(getSenderIP(), getSenderPort(),
					receiveValue<GameMember>().m_name, i + 1)));
			//tell the new member his id
			sendUdpMessege<int>(memberId, i + 1, getSenderIP(), getSenderPort());
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
	for (int i = 1; i < MAX_SERVER_PLAYERS; ++i) {
		if (getMembers(i))
			if (i + 1 != newMember.m_id) {
				sendUdpMessege<AddMember>(addMember, newMember,
					getMembers(i)->m_memberIp, getMembers(i)->m_memberPort);
			}
	}
}
/*============================================================================
* The method run the Server. seem like temp
*/
bool Server::run(sf::RenderWindow& window) {
	system("CLS");
	launch();
	while (true) {
		if (receivedMessege()) {
			handleRequests();
			system("CLS");
			for (int i = 0; i < MAX_SERVER_PLAYERS; ++i)
				if (getMembers(i)) {
					std::cout << i + 1 << ". " << getMembers(i)->m_name << std::endl;
				}
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
			for (int i = 1; i < MAX_SERVER_PLAYERS; ++i)
				if (getMembers(i))
					sendNetworkMessege(startGame, getMembers(i)->m_memberIp, getMembers(i)->m_memberPort);
			return true;
		}
	}
	return false;
}
