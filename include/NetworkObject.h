﻿#pragma once
#include <SFML/Network.hpp>
#include <SFML/Graphics.hpp>
#include <MessegesStructs.h>
#include <Macros.h>
#include <vector>
#include <exception>

class Board;

class NetworkObject {
public:
	NetworkObject(unsigned short port = 0);
	virtual ~NetworkObject() = default;
	//========================== inbox checkers ==============================
	bool receivedMessege(float seconds = 0.000001f);
	//====================== pure abstracts methods ==========================
	virtual bool handleRequests(int = 10) = 0;
	virtual void notifyClosing() = 0;
	virtual void updateLoc(const MemberInfo&) = 0;
	virtual bool launch() = 0;
	virtual void sendStaticCollision(int) = 0;
	//============================= gets section =============================
	const sf::IpAddress& getIP() const { return m_ip; }
	unsigned short getPort() const { return m_port; }
	const sf::IpAddress& getSenderIP() const { return m_senderIP; }
	unsigned short getSenderPort() const { return m_senderPort; }
	const GameMember* getMembers(int index)const;
	const GameMember& getInfo()const { return m_info; }
	bool getStarted() { return m_started; }
	//============================= sets section =============================
	virtual void setName(const char name[PLAYER_NAME_LEN], int index = -1);
	void setId(int id);
	void setBoard(Board* board) { m_board = board; }

	//test
	Board* getBoard() { return m_board; }

protected:
	//====================== messeges handeling section ======================
	//sending section
	template <class T>
	void sendMessege(Messege_type, T,
		const sf::IpAddress& ip = sf::IpAddress::None, unsigned short port = 0);
	//receiving section
	template <class T>
	T receiveValue();

	void addMemberToList();
	void updateMember(const MemberInfo& member);
	void setMember(int index, std::unique_ptr<GameMember>);
	void setStarted(bool value) { m_started = value; }
	//=========================== gets section ===============================
	sf::Packet m_packet;

private:
	//last sender info
	sf::IpAddress m_senderIP;
	unsigned short m_senderPort;
	//server communication members
	sf::UdpSocket m_socket;
	sf::IpAddress m_ip;
	unsigned short m_port;
	sf::SocketSelector m_selector;
	//game members
	std::vector<std::unique_ptr<GameMember>> m_members;
	GameMember m_info;
	bool m_started ;

	Board* m_board;
	void receiveUdp() { m_socket.receive(m_packet, m_senderIP, m_senderPort); }
	void sendUdp(const sf::IpAddress& ip, unsigned short port);
};
/*==========================================================================*/
template<class T>
 void NetworkObject::sendMessege(Messege_type type,T value,const sf::IpAddress& ip
	 , unsigned short port){
	m_packet.clear();
	m_packet << type;
	int size = sizeof(T);
	m_packet.append(&value, sizeof(T)); ;
	(ip == sf::IpAddress::None || port == 0) ? sendUdp(m_senderIP, m_senderPort) 
		: sendUdp(ip, port);
	m_packet.clear();
}
 /*===========================================================================
 * The mehod receiving the received messege value form the m_packet.
 * to know the vlue type, use receiveValue<Messege_type> first.
 */
template<class T>
T NetworkObject::receiveValue(){
	//reading from the last char of the messege type the rest of the messege
	T value = *((T*)(((char*)m_packet.getData()) + sizeof(Messege_type)));
	m_packet.clear();
	return value;
}
/*==========================================================================*/
template<>
Messege_type NetworkObject::receiveValue<Messege_type>();