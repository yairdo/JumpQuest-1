#pragma once
#include "Resources.h"
#include <SFML/Graphics.hpp>
#include "box2d/box2d.h"

constexpr auto BUTTUN_SPACE_PREC = 0.05f;
constexpr auto PREC4LET_STAND = 0.025f;
constexpr auto PREC4LET = 0.022f;
constexpr auto TENTH_PREC = 0.1f;
constexpr auto PREC70 = 0.7f;
constexpr auto THIRD = 3.f;
constexpr auto FORTH = 4.f;
constexpr auto BUT3 = 3;
constexpr auto BUT4 = 4;
constexpr auto SPACES2 = 2;
constexpr auto SPACES3 = 3;
constexpr auto FIRST_BUT = 0;
constexpr auto SEC_BUT = 1;
constexpr auto THIRD_BUT = 2;
constexpr auto FORTH_BUT = 3;
constexpr auto GAME_BACK = 0.6f;
constexpr auto CLIENT_STATE_MSG = 300;
constexpr auto SERVER_STATE_MSG = 200;
constexpr auto UPDATE_TIMER = 0.06f;
constexpr auto EXEPTION_HEIGHT = 250.f;
const sf::Color WHITE_TRANSP_CLR { 255,255,255,120 };
const sf::Color BROWN_TRANSP_CLR{ 102,0,0,150 };
const sf::Color BLACK_TRANSP_CLR{ 0,0,0,150 };


const sf::Vector2f PLAYERS_NAMES_SCALE{ 0.4f,0.8f };



constexpr auto TIME_STEP = 1.0f / 60.0f;
constexpr signed int VEL_ITERS = 6;
constexpr signed int POS_ITERS = 2;
constexpr auto FRAME_RATE = 60;
constexpr auto SCALE = 100.f;
constexpr auto SPACE_BUTTONS = 50.f;
constexpr auto MAIN_MENU_BUTTONS = 4;
constexpr auto MULTI_BUTTONS = 3;
constexpr auto PIX4LET = 45.f;
constexpr auto FOOT = 3;
constexpr auto MUSIC_VOLUME = 10; //was 2
constexpr auto EFFECTS_VOLUME = 10; //was 6
constexpr auto PLAYER_NAME_LEN = 20;
constexpr auto STUN_TIME = 3; //secounds
constexpr auto PLAYER_SPEED = 75.f; //secounds
constexpr auto PLAYER_PROJ_EFFECT = 0.3f; //secounds


const float MAP_SIZE = 6000;
const float PLAYER_PROJECTILE_DIS = 500;
const sf::Vector2f PLAYER_SIZE = { 50,75 };
const sf::Vector2f PROJECTILE_SIZE{25,25};

const sf::Vector2f ARCHER_SIZE{ 40,40 };
const b2Vec2 PROJECTILE_FORCE{ 0.5f,-0.2f };


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

enum MapType {
	castle,
	hell,
	sky,
	menu,
	win,
};

enum TexturesNum {
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
	player0,
	player1,
	player2,
	player3,
	player4,
	player5,
	lobbyBackground,
	start,
	lobbyTitle,
	gameBackground,
	checkPoint,
	mainMenu,
	resume,
	arrow,
	archer,
	chooseBoard,
	block,
	movingBlock,
	gift,
	shooter,
	fallingBlock,
	floorObs,
	rope,
	finishLine,
	loading,
	pleaseWait,
	howTo,
	castleScreenShot,
	hellScreenShot,
	skyScreenShot,
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
enum EntityCategory {
	playerBits = 0x0001,
	giftBits = 0x0002,
	wallBits = 0x0004,
	boundryBits = 0x0008,
	movingBlockBits = 0x0010,
	playerSensorBits = 0x0020,
	ladderBits = 0x0040,
	fallingBlockBits = 0x0080,
	footBits = 0x0100,
	checkPointBits = 0x0200,
	noneBit = 0x0400,
	floorObsBit = 0x0800,
	noHandleBit = 0x1000
};

//============================================================================
//Network Consts
enum NetworkMessages {
	noMessage,
	whoIsAServer,
	iAmAServer,
	iAmFree,
	whoIsFreeServer,
	registered,
	closing,
	startGameMessage,
};
//============================================================================
//in command, the value received after the Message type.
enum MessageType {
	noType, //none
	networkMessage, //Network_message
	gameMembership, //GameMembership
	singMeIn, //GameMember
	memberId, //int 
	addMember, //AddMember
	memberInfo, //MemberInfo
	movingObj, //MemberInfo
	staticObjInfo, //StaticMemberInfo
	closer, //int
	addProjectile, //AddProjectile
	startGame, //StartMessage
	notifyWin, //int
	iAmReady, //unsigned short (id)
	movingObjInfo //MovingObjInfo
};
constexpr auto SERVERS_PORT = 50000;
constexpr auto MAX_SERVER_PLAYERS = 6;
constexpr auto MAX_OBJ_IN_LEVEL = 200;

//exeptions type
constexpr auto RECEIVED_WRONG_MESSEGE_TYPE = "received wrong message type\n";
constexpr auto SOKET_ERROR = "socket error!\n";
constexpr auto SERVER_CONNECTION_LOST = "disconnected from server.";


//for anime
enum animPos {
	walking,
	jumping,
	climb,
	idle,
	stun
};

enum Sounds {
	jumpingSound,
	giftHitSound,
	giftCollectSound,
	checkPointSound,
	teleportSound,
	rocketSound,
};

constexpr float PLAYER_SWITCH_TIME = 0.2f;
constexpr int PLAYER_WIDTH = 130;
constexpr int PLAYER_HEIGHT = 155;

constexpr float FALLING_SWITCH_TIME = 0.05f;
constexpr int FALLING_WIDTH = 200;
constexpr int FALLING_HEIGHT = 200;
constexpr int FALLING_LEN = 8;

constexpr float CHECKPOINT_SWITCH_TIME = 0.15f;
constexpr int CHECKPOINT_WIDTH = 33;
constexpr int CHECKPOINT_HEIGHT = 38;
constexpr int CHECKPOINT_LEN = 6;

constexpr int ARCHER_WIDTH = 160;
constexpr int ARCHER_HEIGHT = 146;
constexpr int ARCHER_LEN = 6;


constexpr int ARROW_WIDTH = 100;
constexpr int ARROW_HEIGHT = 100;
constexpr int ARROW_LEN = 6;

constexpr int GIFT_WIDTH = 100;
constexpr int GIFT_HEIGHT = 55;
constexpr int GIFT_LEN = 3;

constexpr int FLOOR_OBS_WIDTH = 100;
constexpr int FLOOR_OBS_HEIGHT = 200;
constexpr int FLOOR_OBS_LEN = 6;

constexpr int WALKING = 3;
constexpr int STAND = 3;
constexpr int CLIMB = 2;
constexpr int JUMP = 1;

constexpr int ROPE_WIDTH = 200;
constexpr int ROPE_HEIGHT = 200;