#pragma once
#include <unordered_map>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <string>
#include "PairHash.h"
class Resources {
public:
	//----------------functions-------------------
	static Resources& getResourceRef();
	const sf::Texture& getTexture(int index) const;
	const sf::Texture& getTexture(int map,int index) const;
	const sf::Font& getFont(int index) const;
	void playMusic(int);
	void playSound(int);
	
	const int getButLen(int index) const;
private:
	//Singeltone class
	Resources();
	Resources(const Resources&) = default;
	Resources& operator=(const Resources&) = default;
	void setTextures();
	void setButtonStrLengh();
	void setFonts();
	void setGameTextures();
	void setSounds();
	
	std::unordered_map <int, sf::Texture> m_textures;
	std::unordered_map<int, sf::Font> m_fonts;
	std::unordered_map<int, int> m_buttonStrLen;
	std::unordered_map<std::pair<int, int>, sf::Texture,ArgsHash<int>,KeyEqual<int>> m_gameTextures;
	std::unordered_map <int, sf::SoundBuffer> m_sounds;
	sf::Sound m_sound;
	sf::Music m_music;

	template <class SfObj>
	SfObj loadSfObj(const std::string& str);
	template<class Container, class Return>
	Return findInMap(Container map, int index) const;
};
template<class Container, class Return>
Return Resources::findInMap(Container map, int index) const{
	auto it = map.find(index);
	if (it != map.end())
		return (it->second);
	throw std::out_of_range("Texture not found.");
}
template <class SfObj>
SfObj Resources::loadSfObj(const std::string& str) {
	SfObj obj;
	if (obj.loadFromFile(str))
		return std::move(obj);
	throw std::out_of_range("Coldn't find the file");
}