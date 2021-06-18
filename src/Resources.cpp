#include "Resources.h"
#include "Macros.h"
#include <iostream>
using TexturesPair=std::pair<int, sf::Texture> ;
//using TexturesPair = std::pair<int, std::unique_ptr<sf::Texture>>;
using ButtonPairStr = std::pair<int, int>;
using FontPair = std::pair<int, sf::Font>;

Resources::Resources() {
	setTextures();
	setButtonStrLengh();
	setFonts();
	
}
const sf::Texture& Resources::getTexture(int index) const {
	auto it = m_textures.find(index);
	if (it != m_textures.end())
		return it->second;
	throw std::out_of_range("Texture not found.");
	/*auto p = findInMap<std::unordered_map<int, std::unique_ptr<sf::Texture>>, std::unique_ptr<sf::Texture>>
		   (m_textures, index);*/
}
const int Resources::getButLen(int index) const{
	return findInMap<std::unordered_map<int, int>, int>(m_buttonStrLen, index);
}
const sf::Font& Resources::getFont(int index) const {
	auto it = m_fonts.find(index);
	if (it != m_fonts.end())
		return it->second;
	throw std::out_of_range("Texture not found.");
}
void Resources::setTextures() {
	
	m_textures.insert(TexturesPair(menuBackground,
					  createTexture("menuBackground.jpg")));
	m_textures.insert(TexturesPair(title,
		createTexture("title.png")));
	m_textures.insert(TexturesPair(buttonShape,
		createTexture("buttonShape.png")));
	m_textures.insert(TexturesPair(client,
		createTexture("Join Game.png")));
	m_textures.insert(TexturesPair(host,
		createTexture("Host Game.png")));
	m_textures.insert(TexturesPair(singlePlayer,
		createTexture("SinglePlayer.png")));
	m_textures.insert(TexturesPair(help,
		createTexture("How To Play.png")));
	m_textures.insert(TexturesPair(ext,
		createTexture("Exit.png")));
	m_textures.insert(TexturesPair(multiplayer,
		createTexture("Multiplayer.png")));
	m_textures.insert(TexturesPair(back,
		createTexture("Back.png")));
	m_textures.insert(TexturesPair(blank,
		createTexture("Blank.png")));
	m_textures.insert(TexturesPair(player,
		createTexture("elvis.png")));
	m_textures.insert(TexturesPair(lobbyBackground,
		createTexture("lobbyBackground.png")));
	m_textures.insert(TexturesPair(start,
		createTexture("start.png")));
	m_textures.insert(TexturesPair(lobbyTitle,
		createTexture("lobbyTitle.png")));
	m_textures.insert(TexturesPair(block,
		createTexture("bricks.png")));
	m_textures.insert(TexturesPair(castle,
		createTexture("castle map.png")));
	m_textures.insert(TexturesPair(rope,
		createTexture("rope.png")));
	m_textures.insert(TexturesPair(fallingBlock,
		createTexture("trap chandelier.png")));
	m_textures.insert(TexturesPair(checkPoint,
		createTexture("checkPoint.png"))); 
	m_textures.insert(TexturesPair(michal,
		createTexture("michal.png")));

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
	m_fonts.insert(FontPair(lobbyFont, createFont("SNAP.TTF")));
}

sf::Font Resources::createFont(const std::string& str){
	sf::Font font;
	if (!font.loadFromFile(str))
		std::cout << "font not found" << std::endl;
	return std::move(font);
}


sf::Texture Resources::createTexture(const std::string& textureStr) const{
	sf::Texture texture;
	if (!texture.loadFromFile(textureStr))
		std::cout << "texture not found" << std::endl;
	return std::move(texture);
}
//std::unique_ptr<sf::Texture> Resources::createTexture(const std::string& textureStr) const {
//	sf::Texture texture;
//	texture.loadFromFile(textureStr);
//	return std::make_unique<sf::Texture>(texture);
//}
Resources& Resources::getResourceRef() {
	static Resources resources;
	return resources;
}