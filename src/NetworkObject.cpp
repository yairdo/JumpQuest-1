#include <NetworkObject.h>
#include <Macros.h>
#include <MessagesStructs.h>

/*==========================================================================*/
NetworkObject::NetworkObject(unsigned short port):m_ip(sf::IpAddress(sf::IpAddress::getLocalAddress())),
m_socket(), m_selector(), m_packet(), m_senderIP(sf::IpAddress::None), m_mapType(hell),
m_senderPort(0), m_port(port),m_members(MAX_SERVER_PLAYERS), m_started(false),
m_winner(MAX_SERVER_PLAYERS){
	bindSocket(port);
	m_selector.add(m_socket);
	m_packet.clear();
	m_socket.setBlocking(false);
}
/*============================================================================
* The method send the message in the m_packet parameter.
* if no ip or port received. the method will send the message to the last
* person the server received message from.
*/
bool NetworkObject::sendUdp(const sf::IpAddress& ip, unsigned short port, bool validation) {
	bool sent = false;
	do {
		sent = m_socket.send(m_packet, ip, port) == sf::Socket::Done;
	} while (sent != true && validation);
	return sent;
}
/*============================================================================
* The method return if there is message wait in m_socket.
*/
bool NetworkObject::receivedMessage(float seconds) {
	return m_selector.wait(sf::seconds(seconds));
}
/*==========================================================================*/
const GameMember* NetworkObject::getMember(int index) const{
	if (index >= 0 && index < m_members.size())
		if (m_members[index])
			return(m_members[index].get());
	return nullptr;
}
/*==========================================================================*/
void NetworkObject::setName(const char name[PLAYER_NAME_LEN], int index){
	if (index == -1)
		index = m_info.m_info.m_id;
	std::memcpy(m_members[index % MAX_SERVER_PLAYERS]->m_name , name, PLAYER_NAME_LEN);
	if(index == m_info.m_info.m_id)
		std::memcpy(m_info.m_name, name, PLAYER_NAME_LEN);
}
/*============================================================================
* The method is receiving the messages type. every message reading need to
* start with a MessageType receiving first.
*/
template<>
MessageType NetworkObject::receiveValue<MessageType>() {
	receiveUdp();
	int value;
	m_packet >> value;
	return (MessageType)value;
}
/*==========================================================================*/
/*
* adds the names of the other players to the lobby - notify the other players
* that you logged in
*/
void NetworkObject::addMemberToList() {
	AddMember member = receiveValue<AddMember>();
	if (!m_members[member.m_id])
		m_members[member.m_id] = std::make_unique<GameMember>();
	m_members[member.m_id]->m_info.m_id = member.m_id;
	std::memcpy(m_members[member.m_id]->m_name, member.m_name, PLAYER_NAME_LEN);
}
/*==========================================================================*/
void NetworkObject::updateMember(const MemberInfo& member) {
	if (m_members[member.m_id])
		m_members[member.m_id]->m_info = member;
}
/*==========================================================================*/
void NetworkObject::setMember(int index, std::unique_ptr<GameMember> member){
	if (index >= 0 && index < m_members.size())
		m_members[index] = std::move(member);
}
/*==========================================================================*/
void NetworkObject::setId(int id) {
	m_info.m_info.m_id = id;
	if (!getMember(id))
		setMember(id, 
			std::make_unique<GameMember>(GameMember(getIP(), getPort(), "",m_info.m_info)));
}
/*==========================================================================
* The method is binding the soket member to the received port,
* if the soket value is the default value, the method will bind the socket
* to a free port.
*/
void NetworkObject::bindSocket(unsigned short port){
	if (port == 0)
		m_binded = m_socket.bind(sf::Socket::AnyPort, m_ip) == sf::Socket::Done;
	else
		m_binded = m_socket.bind(SERVERS_PORT, m_ip) == sf::Socket::Done;
	if (!m_port) {
		m_port = m_socket.getLocalPort();
	}
	m_socket.setBlocking(false);
	sendMessage<int>(noType, 0, sf::IpAddress::Broadcast, SERVERS_PORT);
	m_binded = receivedMessage();
}