#pragma once
#include <typeindex>
#include "GameObj.h"
#include <unordered_map>
#include "PairHash.h"

using Key = std::pair< std::type_index,  std::type_index>;

class CollisionHandler {
	
public:
	using CorrectFunc = void(CollisionHandler::*)(GameObj*, GameObj*) const;
	static CollisionHandler& getRef() ;
	void handleCollision(GameObj* obj1, GameObj* obj2);
private:
	CollisionHandler();
	CollisionHandler(const CollisionHandler&) = default;
	CollisionHandler& operator=(const CollisionHandler&) = default;
	
	std::unordered_map<Key, CorrectFunc, ArgsHash<std::type_index>, KeyEqual<std::type_index>> m_collisionMap;
	void playerGift(GameObj*,GameObj*) const;
	void giftPlayer(GameObj*, GameObj*)const;
	void playerRope(GameObj*, GameObj*)const;
	void ropePlayer(GameObj*, GameObj*)const;
	void playerMovingBlock(GameObj*, GameObj*)const;
	void movingBlockPlayer(GameObj*, GameObj*)const;
	void checkPointPlayer(GameObj*, GameObj*)const;
	void playerCheckPoint(GameObj*, GameObj*)const;
	void playerProjectile(GameObj*, GameObj*)const;
	void projectilePlayer(GameObj*, GameObj*)const;
	void fallingBlockBlock(GameObj*, GameObj*)const;
	void blockFallingBlock(GameObj*, GameObj*)const;
	void playerFloorObstacle(GameObj*, GameObj*)const;
	void floorObstaclePlayer(GameObj*, GameObj*)const;
	void fallingBlockPlayer(GameObj*, GameObj*)const;
	void playerFallingBlock(GameObj*, GameObj*)const;
};
