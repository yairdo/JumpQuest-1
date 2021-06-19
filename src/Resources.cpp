#include "Resources.h"
#include "Macros.h"
#include <iostream>
using TexturesPair=std::pair<int, sf::Texture> ;
using ButtonPairStr = std::pair<int, int>;
using FontPair = std::pair<int, sf::Font>;

Resources::Resources() {
	setTextures();
	setButtonStrLengh();
	setFonts();
	
}
const sf::Texture& Resources::getTexture(int index) const {
	auto it = m_textures.find(index);
	std::cout << typeid(it).name() << std::endl;
	if (it != m_textures.end())
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
	m_textures.insert(TexturesPair(player,
		loadSfObj<sf::Texture>("player1.png")));
	m_textures.insert(TexturesPair(lobbyBackground,
		loadSfObj<sf::Texture>("lobbyBackground.png")));
	m_textures.insert(TexturesPair(start,
		loadSfObj<sf::Texture>("start.png")));
	m_textures.insert(TexturesPair(lobbyTitle,
		loadSfObj<sf::Texture>("lobbyTitle.png")));
	m_textures.insert(TexturesPair(block,
		loadSfObj<sf::Texture>("bricks.png")));
	m_textures.insert(TexturesPair(castle,
		loadSfObj<sf::Texture>("castle map.png")));
	m_textures.insert(TexturesPair(rope,
		loadSfObj<sf::Texture>("rope.png")));
	m_textures.insert(TexturesPair(fallingBlock,
		loadSfObj<sf::Texture>("trap chandelier.png")));
	m_textures.insert(TexturesPair(checkPoint,
		loadSfObj<sf::Texture>("checkPoint.png")));
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

}

void Resources::setFonts(){
	m_fonts.insert(FontPair(lobbyFont, loadSfObj<sf::Font>("SNAP.TTF")));
}

Resources& Resources::getResourceRef() {
	static Resources resources;
	return resources;
}