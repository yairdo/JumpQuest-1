#include <NetworkObject.h>
#include <iostream>
#include <Macros.h>
/*==========================================================================*/
NetworkObject::NetworkObject(unsigned short port):m_ip(sf::IpAddress::getLocalAddress()), m_udpSocket(), 
m_udpSelector(), m_packet(), m_senderIP(sf::IpAddress::None), m_senderPort(0), m_port(port), m_tcpSocket(),
m_members(MAX_SERVER_PLAYERS), m_listener(), m_tcpSelector() {
	if (port == 0)
		m_udpSocket.bind(sf::Socket::AnyPort,m_ip);
	else
		m_udpSocket.bind(SERVERS_PORT, m_ip);
	if (!m_port) {
		m_port = m_udpSocket.getLocalPort();
	}
	m_listener.listen(SERVERS_PORT);
	m_udpSelector.add(m_udpSocket);
	m_tcpSelector.add(m_tcpSocket);
	m_packet.clear();
	m_tcpSocket.setBlocking(false);
	m_udpSocket.setBlocking(false);
}
/*============================================================================
* The method send the messege in the m_packet parameter, Tcp protocol.
* if no ip, or port received. the method will send the messege to the last
* person the server received messege from.
*/
void NetworkObject::sendTcp(sf::TcpSocket& socket) {
	std::cout << "tcp messege sent.\n";
	while (socket.send(m_packet) != sf::Socket::Done);
}
/*============================================================================
* The method send the messege in the m_packet parameter.
* if no ip or port received. the method will send the messege to the last
* person the server received messege from.
*/
void NetworkObject::sendUdp(const sf::IpAddress& ip, unsigned short port) {
	std::cout << "udp messege sent.\n";
	while (m_udpSocket.send(m_packet, ip, port) != sf::Socket::Done);
}
/*============================================================================
* The method return if there is messege wait in m_socket.
*/
bool NetworkObject::receivedUdpMessege(float seconds) {
	return m_udpSelector.wait(sf::seconds(seconds));
}
/*==========================================================================*/
bool NetworkObject::receivedTcpMessege(float seconds) {
	return m_tcpSelector.wait(sf::seconds(seconds));
}
/*==========================================================================*/
const GameMember* NetworkObject::getMembers(int index) const
{
	if (index >= 0 && index < m_members.size())
		if (m_members[index])
			return(m_members[index].get());
	return nullptr;
}
/*==========================================================================*/
void NetworkObject::setName(const char name[PLAYER_NAME_LEN], int index){
	if (index == -1)
		index = m_info.m_id;
	std::memcpy(m_members[index % MAX_SERVER_PLAYERS]->m_name , 
		name, PLAYER_NAME_LEN);
}
/*============================================================================
* The method is receiving the messeges type. every messege reading need to
* start with a Messege_Type receiving first.
*/
template<>
Messege_type NetworkObject::receiveUdpValue<Messege_type>() {
	receiveUdp();
	int value;
	m_packet >> value;
	return (Messege_type)value;
}
/*==========================================================================*/
template<>
Messege_type NetworkObject::receiveTcpValue<Messege_type>() {
	receiveTcp(m_tcpSocket);
	int value;
	m_packet >> value;
	return (Messege_type)value;
}
/*==========================================================================*/
void NetworkObject::addMemberToList() {
	AddMember member = receiveUdpValue<AddMember>();
	if (!m_members[member.m_id - 1])
		m_members[member.m_id - 1] = std::make_unique<GameMember>();
	m_members[member.m_id - 1]->m_id = member.m_id;
	std::memcpy(m_members[member.m_id - 1]->m_name, member.m_name, PLAYER_NAME_LEN);
}
/*==========================================================================*/
void NetworkObject::updateMember(const MemberInfo& member) {
	if (m_members[member.m_id - 1]) {
		m_members[member.m_id - 1]->m_id = member.m_id;
		m_members[member.m_id - 1]->m_loc = member.m_loc;
		m_members[member.m_id - 1]->m_state = member.state;
	}
}/*==========================================================================*/
void NetworkObject::setMember(int index, std::unique_ptr<GameMember> member){
	std::cout << "id: " << member->m_id;
	if (index >= 0 && index < m_members.size())
		m_members[index] = std::move(member);
}