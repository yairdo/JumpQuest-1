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
	static CollisionHandler& getRef() ;
	void handleCollision(GameObj* obj1, GameObj* obj2);
private:
	CollisionHandler();
	CollisionHandler(const CollisionHandler&) = default;
	CollisionHandler& operator=(const CollisionHandler&) = default;
	
	std::unordered_map<Key, CorrectFunc, ArgsHash<std::type_index>, KeyEqual<std::type_index>> m_collisionMap;
	void playerGift(GameObj*,GameObj*);
	void giftPlayer(GameObj*, GameObj*);
	void playerRope(GameObj*, GameObj*);
	void ropePlayer(GameObj*, GameObj*);
	void playerMovingBlock(GameObj*, GameObj*);
	void movingBlockPlayer(GameObj*, GameObj*);
	void checkPointPlayer(GameObj*, GameObj*);
	void playerCheckPoint(GameObj*, GameObj*);
	void playerProjectile(GameObj*, GameObj*);
	void projectilePlayer(GameObj*, GameObj*);
	void fallingBlockBlock(GameObj*, GameObj*);
	void blockFallingBlock(GameObj*, GameObj*);
	void playerFloorObstacle(GameObj*, GameObj*);
	void floorObstaclePlayer(GameObj*, GameObj*);
	void fallingBlockPlayer(GameObj*, GameObj*);
	void playerFallingBlock(GameObj*, GameObj*);
};
