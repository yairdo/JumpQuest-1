#pragma once
#include "Resources.h"
#include <SFML/Graphics.hpp>
constexpr auto TIME_STEP = 1.0f / 60.0f;
constexpr signed int VEL_ITERS = 6;
constexpr signed int POS_ITERS = 2;
constexpr auto FRAME_RATE = 60;
constexpr auto SCALE = 100.f;
constexpr auto SPACE_BUTTONS = 50.f;
constexpr auto MAIN_MENU_BUTTONS = 4;
constexpr auto MULTI_BUTTONS = 3;
constexpr auto PIX4LET = 45.f;
constexpr auto PLAYER_NAME_LEN = 20;
constexpr auto FOOT = 3;
constexpr auto MAX_LIST_NAMES_SIZE=4;

const float PLAYER_PROJECTILE_DIS = 300;
const sf::Vector2f PROJECTILE_SIZE{ 10,10 };


enum direction {
	left,
	right,
	up,
	down,
	none
};

//enum objectType {
//	block,
//	movingBlock,
//	gift,
//	shooter,
//	fallingBlock,
//	floorObs,
//	rope
//};


enum TexturesNum {
	block,
	movingBlock,
	gift,
	shooter,
	fallingBlock,
	floorObs,
	rope,
	menuBackground,
	title,
	buttonShape,
	singlePlayer,
	host,
	client,
	help,
	ext,
	back,
	multiplayer,
	blank,
	player,
	//player2,
	//player3,
	//player4,
	//player5,
	lobbyBackground,
	start,
	lobbyTitle,
	castle,
	checkPoint,
	mainMenu,
	resume,
	nullpt
};

enum FontsType {
	lobbyFont
};

//enum class ButtonNum {
//	singlePlayer,
//	host,
//	client,
//	help,
//	exit
//};
enum _entityCategory {
	playerBits = 0x0001,
	giftBits = 0x0002,
	wallBits = 0x0004,
	boundryBits = 0x0008,
	movingBlockBits = 0x00010,
	playerSensorBits = 0x00020,
	ladderBits = 0x00040,
	fallingBlockBits = 0x00080,
	footBits = 0x000100,
	checkPointBits = 0x000200,
	noneBit = 0x000400
};

//============================================================================
//Network Consts
enum Network_messeges {
	noMessege,
	whoIsAServer,
	iAmAServer,
	iAmFree,
	whoIsFreeServer,
	registered,
	closing,
	startGame,
};
enum Messege_type {
	noType, //none
	networkMessege, //Network_messege
	gameMembership, //GameMembership
	singMeIn,
	memberId, //int 
	addMember, //AddMember
	memberInfo, //MemberInfo
	movingObj, //MemberInfo
	staticObjInfo, //StaticMemberInfo
	closer, //int
};
constexpr auto SERVERS_PORT = 50000;
constexpr auto NETWORK_MESSEGE_LEN = sizeof(Network_messeges);
constexpr auto MAX_SERVERS_NUM = 1;
constexpr auto MAX_SERVER_PLAYERS = 5;
constexpr auto MAX_OBJ_IN_LEVEL = 200;

//exeptions type
constexpr auto RECEIVED_WRONG_MESSEGE_TYPE = "received wrong messege type\n";
constexpr auto SOKET_ERROR = "socket error!\n";
constexpr auto SERVER_CONNECTION_LOST = "disconnected from server.";



//for anime
enum animPos {
	walking,
	jumping,
	climb,
	idle
};
constexpr float PLAYER_SWITCH_TIME = 0.15;
constexpr int PLAYER_WIDTH = 130;
constexpr int PLAYER_HEIGHT = 155;

constexpr float FALLING_SWITCH_TIME = 0.05;
constexpr int FALLING_WIDTH = 200;
constexpr int FALLING_HEIGHT = 193;
constexpr int FALLING_LEN = 8;

constexpr float CHECKPOINT_SWITCH_TIME = 0.15;
constexpr int CHECKPOINT_WIDTH = 33;
constexpr int CHECKPOINT_HEIGHT = 38;
constexpr int CHECKPOINT_LEN = 6;

constexpr int WALKING = 3;
constexpr int STAND = 3;
constexpr int CLIMB = 2;
constexpr int JUMP = 1;

struct ClonePlayer {
	ClonePlayer() = default;
	ClonePlayer(int id):m_sprite(Resources::getResourceRef().getTexture(player)){
	m_sprite.setTextureRect(sf::IntRect(0, 0, PLAYER_WIDTH, PLAYER_HEIGHT));
	m_sprite.setScale(50 / m_sprite.getGlobalBounds().width, 50 / m_sprite.getGlobalBounds().height);
    m_sprite.setOrigin(m_sprite.getTextureRect().width / 2.f, m_sprite.getTextureRect().height / 2.f);
	m_sprite.setPosition(50,50);}
	sf::Sprite m_sprite;
	int m_row;
	int m_col;
	int m_direction;
};