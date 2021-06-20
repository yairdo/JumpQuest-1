#pragma once
#include <map>
#include <typeindex>
#include "GameObj.h"
#include <unordered_map>
#include "PairHush.h"

using Key = std::pair< std::type_index,  std::type_index>;

class CollisionHandler {
	
public:
	
	using CorrectFunc = void(CollisionHandler::*)(GameObj*, GameObj*);
	//typedef void(CollisionHandler::* CorrectFunc)(GameObj*, GameObj*);
	static CollisionHandler& getRef() ;
	void handleCollision(GameObj* obj1, GameObj* obj2);

	//the hash is not good when there are many same object collisons

	//struct ArgsHash {
	//	std::size_t operator () ( const Key& p) const {
	//		std::size_t h1 = std::hash<std::type_index>()(p.first);
	//		std::size_t h2 = std::hash<std::type_index>()(p.second);
	//		return h1 ^ h2;
	//	}
	//};

	//struct KeyEqual
	//{
	//	bool operator()(const Key& a1, const Key& a2) const
	//	{
	//		return (a1.first == a2.first && a1.second == a2.second) ||
	//			(a1.first == a2.second && a1.second == a2.first);
	//	}
	//};
	//void playerGift(GameObj*, GameObj*);
	//void giftPlayer(GameObj*, GameObj*);
	//void playerRope(GameObj*, GameObj*);
	//void ropePlayer(GameObj*, GameObj*);
	//void playerBlock(GameObj*, GameObj*);
	//void blockPlayer(GameObj*, GameObj*);
private:
	CollisionHandler();
	CollisionHandler(const CollisionHandler&) = default;
	CollisionHandler& operator=(const CollisionHandler&) = default;
	
	//std::map<Key, CorrectFunc> m_collisionMap;
	std::unordered_map<Key, CorrectFunc, ArgsHash<std::type_index>, KeyEqual<std::type_index>> m_collisionMap;
	void playerGift(GameObj*,GameObj*);
	void giftPlayer(GameObj*, GameObj*);
	void playerRope(GameObj*, GameObj*);
	void ropePlayer(GameObj*, GameObj*);
	void playerBlock(GameObj*, GameObj*);
	void blockPlayer(GameObj*, GameObj*);
	void checkPointPlayer(GameObj*, GameObj*);
	void playerCheckPoint(GameObj*, GameObj*);
	void playerProjectile(GameObj*, GameObj*);
	void projectilePlayer(GameObj*, GameObj*);
};
