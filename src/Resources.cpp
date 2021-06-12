#include "Resources.h"
#include "Macros.h"
#include <iostream>
using TexturesPair=std::pair<int, sf::Texture> ;
//using TexturesPair = std::pair<int, std::unique_ptr<sf::Texture>>;
using ButtonPairStr = std::pair<int, int>;

Resources::Resources() {
	setTextures();
	setButtonStrLengh();
	
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
}
sf::Texture Resources::createTexture(const std::string& textureStr) const{
	sf::Texture texture;
	if (!texture.loadFromFile(textureStr))
		std::cout << "texture not found" << std::endl;
	return texture;
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