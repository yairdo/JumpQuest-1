#include <NetworkObject.h>
#include <iostream>
#include <Macros.h>
//93,173,114,170 //sf::IpAddress::getLocalAddress()//25,72,141,58
/*==========================================================================*/
NetworkObject::NetworkObject(unsigned short port):m_ip(sf::IpAddress(sf::IpAddress::getLocalAddress())),
m_socket(), m_selector(), m_packet(), m_senderIP(sf::IpAddress::None),
m_senderPort(0), m_port(port),m_members(MAX_SERVER_PLAYERS), m_started(false)
{
	if (port == 0)
		m_socket.bind(sf::Socket::AnyPort,m_ip);
	else
		m_socket.bind(SERVERS_PORT, m_ip);
	if (!m_port) {
		m_port = m_socket.getLocalPort();
	}
	m_selector.add(m_socket);
	m_packet.clear();
	m_socket.setBlocking(false);
}
/*============================================================================
* The method send the messege in the m_packet parameter.
* if no ip or port received. the method will send the messege to the last
* person the server received messege from.
*/
void NetworkObject::sendUdp(const sf::IpAddress& ip, unsigned short port) {
	m_socket.send(m_packet, ip, port) != sf::Socket::Done;
}
/*============================================================================
* The method return if there is messege wait in m_socket.
*/
bool NetworkObject::receivedMessege(float seconds) {
	return m_selector.wait(sf::seconds(seconds));
}
/*==========================================================================*/
const GameMember* NetworkObject::getMembers(int index) const{
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
	std::memcpy(m_info.m_name , name, PLAYER_NAME_LEN);
}
/*============================================================================
* The method is receiving the messeges type. every messege reading need to
* start with a Messege_Type receiving first.
*/
template<>
Messege_type NetworkObject::receiveValue<Messege_type>() {
	receiveUdp();
	int value;
	m_packet >> value;
	return (Messege_type)value;
}
/*==========================================================================*/
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
	if (!getMembers(id))
		setMember(id, 
			std::make_unique<GameMember>(gameMemberCreator(getIP(), getPort(), "", memberInfoCreator(id))));
	//why not send m_info??
}