#pragma once
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

enum MenuType {
	mainMenu,
	multiplayerMenu
};

enum direction {
	left,
	right,
	up,
	down
};
enum objectType {
	block,
	movingBlock,
	gift,
	shooter,
	fallingBlock,
	floorObs,
	rope
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
	player,
	nullpt
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
	ladderBits = 0x00030,
	fallingBlockBits = 0x00040,
	footBits = 0x00050
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
	noType, //NULL
	networkMessege, //Network_messege
	gameMembership, //GameMembership
	singMeIn,
	memberId, //int
	whoElsePlaying,
	addMember,
	memberInfo, //MemberInfo
};
constexpr auto SERVERS_PORT = 50000;
constexpr auto NETWORK_MESSEGE_LEN = sizeof(Network_messeges);
constexpr auto MAX_SERVERS_NUM = 4;
constexpr auto MAX_SERVER_PLAYERS = 5;

//exeptions type
constexpr auto RECEIVED_WRONG_MESSEGE_TYPE = "received wrong messege type\n";
constexpr auto SOKET_ERROR = "socket error!\n";