#pragma once
#include <GameObj.h>
#include<unordered_map>
template<class Type>
class Factory {
public:
	using pFnc = std::unique_ptr<Type>(*)(b2World& world,std::vector<sf::Vector2f>);
	static std::unique_ptr<Type> create(const std::string& name,
		std::vector<sf::Vector2f>, b2World& world);
	static bool registerit(const std::string& name, pFnc);
private:
	static auto& getMap();
};
template<class Type>
std::unique_ptr<Type> Factory<Type>::create(const std::string& name,
	std::vector<sf::Vector2f> vec, b2World& world) {
	auto it = Factory::getMap().find(name);
	if (it == Factory::getMap().end())
		return nullptr;
	return it->second(world, vec);
}

template<class Type>
auto& Factory<Type>::getMap() {
	static std::unordered_map<std::string, pFnc> map;
	return map;
}

template<class Type>
bool Factory<Type>::registerit(const std::string& name, pFnc f) {
	Factory::getMap().emplace(name, f);
	return true;
}