#include "Resources.h"
#include "Macros.h"
#include <iostream>
using TexturesPair=std::pair<int, sf::Texture> ;
using IntPair = std::pair<int, int>;
using GameTexturesPair = std::pair<IntPair, sf::Texture>;

using ButtonPairStr = std::pair<int, int>;
using FontPair = std::pair<int, sf::Font>;

Resources::Resources(): m_buttonStrLen(), m_textures(), m_fonts(),
m_gameTextures(), m_sounds(),m_music(){
	setTextures();
	setGameTextures();
	setButtonStrLengh();
	setFonts();
	setSounds();
	m_music.setVolume(MUSIC_VOLUME);
	m_sound.setVolume(EFFECTS_VOLUME);
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
	m_textures.insert(TexturesPair(lobbyBackground,
		loadSfObj<sf::Texture>("lobbyBackground.png")));
	m_textures.insert(TexturesPair(start,
		loadSfObj<sf::Texture>("start.png")));
	m_textures.insert(TexturesPair(lobbyTitle,
		loadSfObj<sf::Texture>("lobbyTitle.png")));
	m_textures.insert(TexturesPair(castle,
		loadSfObj<sf::Texture>("castle map.png")));
	m_textures.insert(TexturesPair(mainMenu,
		loadSfObj<sf::Texture>("mainMenu.png")));
	m_textures.insert(TexturesPair(resume,
		loadSfObj<sf::Texture>("resume.png")));
	m_textures.insert(TexturesPair(chooseBoard,
		loadSfObj<sf::Texture>("SelectTheme.png")));
	m_textures.insert(TexturesPair(howTo,
		loadSfObj<sf::Texture>("HowTo.png")));
	m_textures.insert(TexturesPair(hellScreenShot,
		loadSfObj<sf::Texture>("hellScreenShot.png")));
	m_textures.insert(TexturesPair(castleScreenShot,
		loadSfObj<sf::Texture>("castleScreenShot.png")));
	m_textures.insert(TexturesPair(skyScreenShot,
		loadSfObj<sf::Texture>("skyScreenShot.png")));

}
void Resources::setGameTextures() {
	m_gameTextures.insert(GameTexturesPair(IntPair(castle, checkPoint),
		loadSfObj<sf::Texture>("checkPoint.png")));
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
	m_gameTextures.insert(GameTexturesPair(IntPair(castle, arrow),
		loadSfObj<sf::Texture>("missle sprite.png")));
	m_gameTextures.insert(GameTexturesPair(IntPair(castle, archer),
		loadSfObj<sf::Texture>("clown castle.png")));
	m_gameTextures.insert(GameTexturesPair(IntPair(castle, gift),
		loadSfObj<sf::Texture>("chest castle.png")));
	m_gameTextures.insert(GameTexturesPair(IntPair(castle, block),
		loadSfObj<sf::Texture>("castle brick.png")));
	m_gameTextures.insert(GameTexturesPair(IntPair(castle, gameBackground),
		loadSfObj<sf::Texture>("castle map.png")));
	m_gameTextures.insert(GameTexturesPair(IntPair(castle, rope),
		loadSfObj<sf::Texture>("castle rope.png")));
	m_gameTextures.insert(GameTexturesPair(IntPair(castle, fallingBlock),
		loadSfObj<sf::Texture>("trap chandelier.png")));
	m_gameTextures.insert(GameTexturesPair(IntPair(castle, floorObs),
		loadSfObj<sf::Texture>("floor spikes.png")));
	m_gameTextures.insert(GameTexturesPair(IntPair(castle, finishLine),
		loadSfObj<sf::Texture>("castle finito.png")));
	m_gameTextures.insert(GameTexturesPair(IntPair(hell, arrow),
		loadSfObj<sf::Texture>("fire ball.png")));
	m_gameTextures.insert(GameTexturesPair(IntPair(hell, archer),
		loadSfObj<sf::Texture>("hell archer.png")));
	m_gameTextures.insert(GameTexturesPair(IntPair(hell, gift),
		loadSfObj<sf::Texture>("chest hell.png")));
	m_gameTextures.insert(GameTexturesPair(IntPair(hell, block),
		loadSfObj<sf::Texture>("lava.jpg")));
	m_gameTextures.insert(GameTexturesPair(IntPair(hell, gameBackground),
		loadSfObj<sf::Texture>("hell map.png")));
	m_gameTextures.insert(GameTexturesPair(IntPair(hell, rope),
		loadSfObj<sf::Texture>("hell rope.png")));
	m_gameTextures.insert(GameTexturesPair(IntPair(hell, fallingBlock),
		loadSfObj<sf::Texture>("lava stone.png")));
	m_gameTextures.insert(GameTexturesPair(IntPair(hell, floorObs),
		loadSfObj<sf::Texture>("lil vulkan.png")));
	m_gameTextures.insert(GameTexturesPair(IntPair(hell, finishLine),
		loadSfObj<sf::Texture>("hell finito.png")));
	m_gameTextures.insert(GameTexturesPair(IntPair(sky, arrow),
		loadSfObj<sf::Texture>("heart arrow.png")));
	m_gameTextures.insert(GameTexturesPair(IntPair(sky, archer),
		loadSfObj<sf::Texture>("sky archer.png")));
	m_gameTextures.insert(GameTexturesPair(IntPair(sky, gift),
		loadSfObj<sf::Texture>("chest sky.png")));
	m_gameTextures.insert(GameTexturesPair(IntPair(sky, block),
		loadSfObj<sf::Texture>("sky brick.png")));
	m_gameTextures.insert(GameTexturesPair(IntPair(sky, gameBackground),
		loadSfObj<sf::Texture>("sky map.png")));
	m_gameTextures.insert(GameTexturesPair(IntPair(sky, rope),
		loadSfObj<sf::Texture>("sky rope.png")));
	m_gameTextures.insert(GameTexturesPair(IntPair(sky, fallingBlock),
		loadSfObj<sf::Texture>("star sprite.png")));
	m_gameTextures.insert(GameTexturesPair(IntPair(sky, floorObs),
		loadSfObj<sf::Texture>("icy spike.png")));
	m_gameTextures.insert(GameTexturesPair(IntPair(sky, finishLine),
		loadSfObj<sf::Texture>("sky finito.png")));

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

Resources& Resources::getResourceRef() {
	static Resources resources;
	return resources;
}
//============================================================================
void Resources::setSounds() {
	m_sounds.insert(std::pair<int, sf::SoundBuffer>(checkPointSound,
		loadSfObj<sf::SoundBuffer>("checkPointSound.wav")));
	m_sounds.insert(std::pair<int, sf::SoundBuffer>(giftCollectSound,
		loadSfObj<sf::SoundBuffer>("giftBreak.wav")));
	m_sounds.insert(std::pair<int, sf::SoundBuffer>(giftHitSound,
		loadSfObj<sf::SoundBuffer>("GiftHit.wav")));
	m_sounds.insert(std::pair<int, sf::SoundBuffer>(jumpingSound,
		loadSfObj<sf::SoundBuffer>("jumpsound.wav")));
	m_sounds.insert(std::pair<int, sf::SoundBuffer>(teleportSound,
		loadSfObj<sf::SoundBuffer>("teleport.wav")));
	m_sounds.insert(std::pair<int, sf::SoundBuffer>(rocketSound,
		loadSfObj<sf::SoundBuffer>("rocket.wav")));
}
//============================================================================
void Resources::playMusic(int type) {
	switch (type) {
	case menu:
		m_music.openFromFile("menu.wav");
		break;
	case hell:
		m_music.openFromFile("lavaLvl.ogg");
		break;
	case sky:
		m_music.openFromFile("skyLvl.wav");
		break;
	case castle:
		m_music.openFromFile("castleMusic.ogg");
		break;
	case win:
		m_music.openFromFile("winSound.ogg");
	}
	m_music.play();
	m_music.setLoop(true);
}
//============================================================================
void Resources::playSound(int pos) {
	m_sound.setBuffer(m_sounds.find(pos)->second);
	m_sound.play();
}