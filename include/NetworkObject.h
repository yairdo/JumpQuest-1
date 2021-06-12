#pragma once
#include <SFML/Network.hpp>
#include <MessegesStructs.h>
#include <Macros.h>
#include <vector>
#include <exception>
#include <SFML/Graphics.hpp>

class NetworkObject {
public:
	NetworkObject(unsigned short port = 0);
	virtual ~NetworkObject() = default;
	bool receivedMessege(float seconds = 0.0001f);
	virtual bool handleRequests(int=1) = 0;
	virtual void notifyClosing() = 0;
	virtual void updateLoc(const sf::Vector2f&, int) = 0;
	const GameMember* getMembers(int index)const;
	const GameMember& getInfo()const { return m_info; }
	void setName(const char name[PLAYER_NAME_LEN]);
	virtual bool run(sf::RenderWindow&)=0;

	const sf::IpAddress& getIP() const { return m_ip; }
	unsigned short getPort() const { return m_port; }
	const sf::IpAddress& getSenderIP() const { return m_senderIP; }
	unsigned short getSenderPort() const { return m_senderPort; }
protected:
	//====================== messeges handeling section ======================
	//sending section
	void sendNetworkMessege(Network_messeges,
		const sf::IpAddress& ip = sf::IpAddress::None, unsigned short port = 0);
	template <class T>
	void sendUdpMessege(Messege_type,T ,const sf::IpAddress& ip = sf::IpAddress::None, unsigned short port = 0);
	template <class T>
	void sendTcpMessege(Messege_type, T, const sf::IpAddress& ip = sf::IpAddress::None, unsigned short port = 0);
	//receiving section
	Messege_type receiveType();
	template <class T>
	T receiveValue();
	void setId(int id) { m_info.m_id = id; }
	void addMemberToList();
	void updateMember(const MemberInfo& member);
	void setMember(int index, std::unique_ptr<GameMember>);

	//=========================== gets section ===============================

	sf::Packet m_packet;
private:
	//last sender info
	sf::IpAddress m_senderIP;
	unsigned short m_senderPort;
	//server communication members
	sf::UdpSocket m_udpSocket;
	sf::TcpSocket m_tcpSocket;
	sf::IpAddress m_ip;
	unsigned short m_port;
	sf::SocketSelector m_selector;
	//game members
	std::vector<std::unique_ptr<GameMember>> m_members;
	GameMember m_info;


	void receiveTcp() { m_tcpSocket.receive(m_packet);}
	void sendTcp(const sf::IpAddress& ip, unsigned short port);
	void receiveUdp() { m_udpSocket.receive(m_packet, m_senderIP, m_senderPort); }
	void sendUdp(const sf::IpAddress& ip, unsigned short port);
};
/*==========================================================================*/
template<class T>
 void NetworkObject::sendUdpMessege(Messege_type type,T value,const sf::IpAddress& ip
	 , unsigned short port){
	m_packet.clear();
	m_packet << type;
	int size = sizeof(T);
	m_packet.append(&value, sizeof(T)); ;
	(ip == sf::IpAddress::None || port == 0) ? sendUdp(m_senderIP, m_senderPort) 
		: sendUdp(ip, port);
	m_packet.clear();
}
 /*==========================================================================*/
 template<class T>
 void NetworkObject::sendTcpMessege(Messege_type type, T value, const sf::IpAddress& ip
	 , unsigned short port) {
	 m_packet.clear();
	 m_packet << type;
	 int size = sizeof(T);
	 m_packet.append(&value, sizeof(T)); ;
	 (ip == sf::IpAddress::None || port == 0) ? sendUdp(m_senderIP, m_senderPort)
		 : sendTcp(ip, port);
	 m_packet.clear();
 }
 /*===========================================================================
 * The mehod receiving the received messege value form the m_packet.
 * to know the vlue type, use receiveValue<Messege_type> first.
 */
template<class T>
T NetworkObject::receiveValue(){
	/*T value, reValue;
	char* reverseMessege = nullptr;
	reverseMessege = new(std::nothrow) char[m_packet.getDataSize() / sizeof(char)];
	for (int i = 0; i < m_packet.getDataSize() / sizeof(char); ++i)
		reverseMessege[i] = ((char*)m_packet.getData())[m_packet.getDataSize() / sizeof(char) - i];
	for (int i = 0; i < m_packet.getDataSize() / sizeof(char); ++i) {
		if (i % 2 == 1 && m_packet.getDataSize() / sizeof(char) != i + 1) {
			char temp = reverseMessege[i];
			reverseMessege[i] = reverseMessege[i + 1];
			reverseMessege[i + 1] = temp;
		}
	}
	for (int i = 0; i < m_packet.getDataSize(); ++i) { // for debug errors
		value = *((T*)(((char*)m_packet.getData()) + i));
		reValue = *((T*)(reverseMessege + i));
	}*/
	//reading from the last char of the messege type the rest of the messege
	T value = *((T*)(((char*)m_packet.getData()) + sizeof(Messege_type)));
	m_packet.clear();
	//	delete[] reverseMessege;
	return value;
}
template<>
Messege_type NetworkObject::receiveValue<Messege_type>();