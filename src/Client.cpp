#include <Client.h>
#include <Board.h>

//-----------------------------------------------------------------------------
Client::Client() : NetworkObject(), m_serverIP(), m_servers(), 
m_isLinked(false), m_gameStarted(false){
}
//============================================================================
Client::~Client() {
	notifyClosing();
}
/*============================================================================
* The method is receiving all the messages the client received and handle them
* as needed.
*/
bool Client::handleRequests(int max) {
	int counter = 0;

	while (receivedMessage()&& counter++ < max) {
			switch (receiveValue<MessageType>())
			{
			case networkMessage:
				handleNetworkMessage();
				break;
			case memberId:
				regesterServer();
				break;
			case addMember:
				addMemberToList();
				break;
			case memberInfo:
				updateMember(receiveValue<MemberInfo>());
				break;
			case movingObj:
				updateMovingObj();
				break;
			case staticObjInfo:
				getBoard()->updateStaticMsgCollision(receiveValue<StaticObjInfo>().m_index);
				break;
			case MessageType::addProjectile:
				getBoard()->addProjectile(receiveValue<AddProjectileMessage>());
				break;
			case closer:
				setMember(receiveValue<int>(), nullptr);
				break;
			case startGame:
				setLvlInfo(receiveValue<MapType>());
				setStarted(true);
				break;
			case notifyWin:
				setWinner(receiveValue<unsigned short>());
				break;
			default:
				break;
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
	sendMessage(networkMessage, whoIsFreeServer, 
		sf::IpAddress::Broadcast, SERVERS_PORT, false);
}
/*============================================================================
* The method notify the host Server that the client is disconnecting.
*/
void Client::notifyClosing() {
	sendMessage<int>(closer, getInfo().m_info.m_id, m_serverIP, SERVERS_PORT, m_isLinked);
}
/*==========================================================================
* The method is uplate the server about the client player location.
*/
void Client::updateLoc( const MemberInfo& member){
	sendMessage<MemberInfo>(memberInfo, member, m_serverIP, SERVERS_PORT);
}
/*==========================================================================
* The method is binding the NetworkObject's socket and preper the obj
* to start communicate with the server.
*/
bool Client::launch()
{
	if (!receivedMessage()){
		searchForServers();
	}
	return getInfo().m_info.m_id != 0;
}
/*============================================================================
* The method is regester saving the member id in the server and the server's
* info to communicate with.
*/
void Client::regesterServer() {
	setId(receiveValue<int>());
	m_serverIP = getSenderIP();
	m_isLinked = true;
}
/*============================================================================
* The method sets a new name for the client and singIn to the server.
*/
void Client::setName(const char name[PLAYER_NAME_LEN], int index) {
	NetworkObject::setName(name);
	sendMessage<GameMember>(singMeIn, getInfo(), m_serverIP, SERVERS_PORT, true);
}
/*============================================================================
* The method is notify the server about collision with static obj
*/
void Client::sendStaticCollision(int index){
	sendMessage<StaticObjInfo>(staticObjInfo, 
		StaticObjInfo(getInfo().m_info.m_id, index));
}
/*============================================================================
* The method is notify the server about collision with moving obj
*/
void Client::updateSingleMovingObjInfo(const MovingObjInfo& info) {
	sendMessage<MovingObjInfo>(movingObjInfo,info);
}
/*============================================================================
* The method is update the Board's moving objects as the server reported.
*/
void Client::updateMovingObj() {
	MovingObjMembersRoport message = receiveValue<MovingObjMembersRoport>();
	for (int i = 0; i < message.m_size; ++i)
		getBoard()->setInfo(i + 1, message.m_locs[i]);
}
/*============================================================================
* The method is singIn to the server.
*/
void Client::sendGameMembership(const char name[]) {
	sendMessage<GameMember>(singMeIn, GameMember(getIP(), getPort(), name, true));
}
/*============================================================================
* If received NetworkMessage, the method is handling the message as needed.
*/
void Client::handleNetworkMessage(){
	switch (receiveValue<NetworkMessages>()) {
	case iAmFree:
		sendGameMembership("client");
		break;
	case closing:
		throw std::exception(SERVER_CONNECTION_LOST);
		break;
	case NetworkMessages::startGameMessage:
		m_gameStarted = true;
		break;
	default:
		break;
	}
}
/*============================================================================*/
/*
* telling the other players that u picked a gift so they will also add a projectile
*/
void Client::addProjectile(const AddProjectileMessage& projectile){
	sendMessage<AddProjectileMessage>(MessageType::addProjectile, projectile, 
		m_serverIP, SERVERS_PORT, true);
}
/*============================================================================*/
/*
* telling the server(which sends the other clients) that he won
*/
void Client::notifyWinning(unsigned short winner){
	sendMessage<unsigned short>(notifyWin, getInfo().m_info.m_id, m_serverIP, 
		SERVERS_PORT, true);
}
/*============================================================================*/
/*
* telling the server he is connected and ready to start
*/
void Client::sendImReady(){
	sendMessage<int>(iAmReady, getInfo().m_info.m_id, m_serverIP, SERVERS_PORT, true);
}
