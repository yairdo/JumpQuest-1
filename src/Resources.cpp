#include "Resources.h"
#include "Macros.h"
#include <iostream>
using TexturesPair=std::pair<int, sf::Texture> ;
using IntPair = std::pair<int, int>;
using GameTexturesPair = std::pair<IntPair, sf::Texture>;

using ButtonPairStr = std::pair<int, int>;
using FontPair = std::pair<int, sf::Font>;

Resources::Resources() {
	setTextures();
	setGameTextures();
	setButtonStrLengh();
	setFonts();
}
const sf::Texture& Resources::getTexture(int index) const {
	auto it = m_textures.find(index);
	if (it != m_textures.end())
		return it->second;
	throw std::out_of_range("Texture not found.");
}
const sf::Texture& Resources::getTexture(int map, int index) const {
	auto it = m_gameTextures.find(std::pair(map,index));
	if (it != m_gameTextures.end())
		return it->second;
	throw std::out_of_range("Texture not found.");
}

const int Resources::getButLen(int index) const{
	return findInMap<std::unordered_map<int, int>, int>(m_buttonStrLen, index);
}
const sf::Font& Resources::getFont(int index) const {
	auto it = m_fonts.find(index);
	std::cout << typeid(it).name() << std::endl;
	if (it != m_fonts.end())
		return it->second;
	throw std::out_of_range("Texture not found.");
}
void Resources::setTextures() {
	
	m_textures.insert(TexturesPair(menuBackground,
		loadSfObj<sf::Texture>("menuBackground.jpg")));
	m_textures.insert(TexturesPair(title,
		loadSfObj<sf::Texture>("title.png")));
	m_textures.insert(TexturesPair(buttonShape,
		loadSfObj<sf::Texture>("buttonShape.png")));
	m_textures.insert(TexturesPair(client,
		loadSfObj<sf::Texture>("Join Game.png")));
	m_textures.insert(TexturesPair(host,
		loadSfObj<sf::Texture>("Host Game.png")));
	m_textures.insert(TexturesPair(singlePlayer,
		loadSfObj<sf::Texture>("SinglePlayer.png")));
	m_textures.insert(TexturesPair(help,
		loadSfObj<sf::Texture>("How To Play.png")));
	m_textures.insert(TexturesPair(ext,
		loadSfObj<sf::Texture>("Exit.png")));
	m_textures.insert(TexturesPair(multiplayer,
		loadSfObj<sf::Texture>("Multiplayer.png")));
	m_textures.insert(TexturesPair(back,
		loadSfObj<sf::Texture>("Back.png")));
	m_textures.insert(TexturesPair(blank,
		loadSfObj<sf::Texture>("Blank.png")));
	//m_textures.insert(TexturesPair(player0,
	//	loadSfObj<sf::Texture>("player0.png")));
	//m_textures.insert(TexturesPair(player1,
	//	loadSfObj<sf::Texture>("player1.png")));
	//m_textures.insert(TexturesPair(player2,
	//	loadSfObj<sf::Texture>("player2.png")));
	//m_textures.insert(TexturesPair(player3,
	//	loadSfObj<sf::Texture>("player3.png")));
	//m_textures.insert(TexturesPair(player4,
	//	loadSfObj<sf::Texture>("player4.png")));
	//m_textures.insert(TexturesPair(player5,
	//	loadSfObj<sf::Texture>("player5.png")));
	m_textures.insert(TexturesPair(lobbyBackground,
		loadSfObj<sf::Texture>("lobbyBackground.png")));
	m_textures.insert(TexturesPair(start,
		loadSfObj<sf::Texture>("start.png")));
	m_textures.insert(TexturesPair(lobbyTitle,
		loadSfObj<sf::Texture>("lobbyTitle.png")));
	//m_textures.insert(TexturesPair(block,
	//	loadSfObj<sf::Texture>("bricks.png")));
	m_textures.insert(TexturesPair(castle,
		loadSfObj<sf::Texture>("castle map.png")));
	/*m_textures.insert(TexturesPair(rope,
		loadSfObj<sf::Texture>("rope.png")));*/
	//m_textures.insert(TexturesPair(fallingBlock,
	//	loadSfObj<sf::Texture>("trap chandelier.png")));
	/*m_textures.insert(TexturesPair(checkPoint,
		loadSfObj<sf::Texture>("checkPoint.png")));*/
	m_textures.insert(TexturesPair(mainMenu,
		loadSfObj<sf::Texture>("mainMenu.png")));
	m_textures.insert(TexturesPair(resume,
		loadSfObj<sf::Texture>("resume.png")));
	m_textures.insert(TexturesPair(chooseBoard,
		loadSfObj<sf::Texture>("SelectTheme.png")));
}
void Resources::setGameTextures() {
	m_gameTextures.insert(GameTexturesPair(IntPair(castle, block),
		loadSfObj<sf::Texture>("bricks.png")));
	m_gameTextures.insert(GameTexturesPair(IntPair(castle, gameBackground),
		loadSfObj<sf::Texture>("castle map.png")));
	m_gameTextures.insert(GameTexturesPair(IntPair(castle, rope),
		loadSfObj<sf::Texture>("rope.png")));
	m_gameTextures.insert(GameTexturesPair(IntPair(castle, fallingBlock),
		loadSfObj<sf::Texture>("trap chandelier.png")));
	m_gameTextures.insert(GameTexturesPair(IntPair(castle,player0),
		loadSfObj<sf::Texture>("player0.png")));
	m_gameTextures.insert(GameTexturesPair(IntPair(castle, player1),
		loadSfObj<sf::Texture>("player1.png")));
	m_gameTextures.insert(GameTexturesPair(IntPair(castle, player2),
		loadSfObj<sf::Texture>("player2.png")));
	m_gameTextures.insert(GameTexturesPair(IntPair(castle, player3),
		loadSfObj<sf::Texture>("player3.png")));
	m_gameTextures.insert(GameTexturesPair(IntPair(castle, player4),
		loadSfObj<sf::Texture>("player4.png")));
	m_gameTextures.insert(GameTexturesPair(IntPair(castle, player5),
		loadSfObj<sf::Texture>("player5.png")));
	m_gameTextures.insert(GameTexturesPair(IntPair(castle, blank),
		loadSfObj<sf::Texture>("Blank.png")));
	m_gameTextures.insert(GameTexturesPair(IntPair(castle, checkPoint),
		loadSfObj<sf::Texture>("checkPoint.png")));
	m_gameTextures.insert(GameTexturesPair(IntPair(castle, arrow),
		loadSfObj<sf::Texture>("duck.png")));
	m_gameTextures.insert(GameTexturesPair(IntPair(castle, archer),
		loadSfObj<sf::Texture>("clown castle.png")));
	//m_gameTextures.insert(GameTexturesPair(IntPair(hell, block),
	//	loadSfObj<sf::Texture>("bricks.png")));
	//m_gameTextures.insert(GameTexturesPair(IntPair(hell, gameBackground),
	//	loadSfObj<sf::Texture>("castle map.png")));
	//m_gameTextures.insert(GameTexturesPair(IntPair(hell, rope),
	//	loadSfObj<sf::Texture>("rope.png")));
	//m_gameTextures.insert(GameTexturesPair(IntPair(hell, fallingBlock),
	//	loadSfObj<sf::Texture>("trap chandelier.png")));

	//m_gameTextures.insert(GameTexturesPair(IntPair(sky, block),
	//	loadSfObj<sf::Texture>("bricks.png")));
	//m_gameTextures.insert(GameTexturesPair(IntPair(sky, gameBackground),
	//	loadSfObj<sf::Texture>("castle map.png")));
	//m_gameTextures.insert(GameTexturesPair(IntPair(sky, rope),
	//	loadSfObj<sf::Texture>("rope.png")));
	//m_gameTextures.insert(GameTexturesPair(IntPair(sky, fallingBlock),
	//	loadSfObj<sf::Texture>("trap chandelier.png")));
}
void Resources::setButtonStrLengh() {
	m_buttonStrLen.insert(ButtonPairStr(singlePlayer, strlen("single player")));
	m_buttonStrLen.insert(ButtonPairStr(multiplayer, strlen("multiplayer")));
	m_buttonStrLen.insert(ButtonPairStr(help, strlen("how to play")));
	m_buttonStrLen.insert(ButtonPairStr(ext, strlen("exit")));
	m_buttonStrLen.insert(ButtonPairStr(host, strlen("host game")));
	m_buttonStrLen.insert(ButtonPairStr(client, strlen("join game")));
	m_buttonStrLen.insert(ButtonPairStr(back, strlen("back")));
	m_buttonStrLen.insert(ButtonPairStr(start, strlen("start")));
	m_buttonStrLen.insert(ButtonPairStr(title, strlen("jump quest")));
	m_buttonStrLen.insert(ButtonPairStr(lobbyTitle, strlen("lobby")));
	m_buttonStrLen.insert(ButtonPairStr(mainMenu, strlen("main menu")));
	m_buttonStrLen.insert(ButtonPairStr(resume, strlen("resume")));
	m_buttonStrLen.insert(ButtonPairStr(chooseBoard, strlen("choose theme")));
}

void Resources::setFonts(){
	m_fonts.insert(FontPair(lobbyFont, loadSfObj<sf::Font>("SNAP.TTF")));
}
void Resources::print() {
	static int i = 0;
	std::cout << ++i << std::endl;
}
Resources& Resources::getResourceRef() {
	static Resources resources;
	return resources;
}