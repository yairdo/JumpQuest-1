#pragma once
#include <unordered_map>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <string>
class Resources {
public:
	//----------------functions-------------------
	static Resources& getResourceRef();
	const sf::Texture& getTexture(int index) const;
	const int getButLen(int index) const;
private:
	//Singeltone class
	Resources();
	Resources(const Resources&) = default;
	Resources& operator=(const Resources&) = default;
	sf::Texture createTexture(const std::string&) const;
	//std::unique_ptr<sf::Texture> createTexture(const std::string&) const;
	void setTextures();
	void setButtonStrLengh();

	template<class Container, class Return>
	Return findInMap(Container map, int index) const;
	std::unordered_map <int, sf::Texture> m_textures;
	//std::unordered_map <int, std::unique_ptr<sf::Texture>> m_textures;
	std::unordered_map<int, int> m_buttonStrLen;
	std::unordered_map <int,std::pair<sf::SoundBuffer, sf::Sound>> m_sounds;
};
template<class Container, class Return>
Return Resources::findInMap(Container map, int index) const{
	auto it = map.find(index);
	if (it != map.end())
		return (it->second);
	throw std::out_of_range("Texture not found.");
}
