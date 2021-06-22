#include <iostream>
#include "CollisionHandler.h"
#include "Gift.h"
#include "Player.h"
#include "Rope.h"
#include "Block.h"
#include "CheckPoint.h"
#include "FallingBlock.h"
#include "FloorObstacle.h"
#include <iostream>
#include <functional>
#include <Macros.h>
#include <Projectile.h>
#include "box2d/box2d.h"

CollisionHandler::CollisionHandler(){
	/*std::function<void(GameObj*, GameObj*)> Func=giftPlayer;
	m_collisionMap[Key(typeid(Gift), typeid(Player))] = &[]() {return &giftPlayer; };*/
	/*auto p = Key(typeid(Gift), typeid(Player));*/
	m_collisionMap[Key(typeid(Gift), typeid(Player))] = &CollisionHandler::giftPlayer;
	m_collisionMap[Key(typeid(Player), typeid(Gift))] = &CollisionHandler::playerGift;
	m_collisionMap[Key(typeid(Rope), typeid(Player))] = &CollisionHandler::ropePlayer;
	m_collisionMap[Key(typeid(Player), typeid(Rope))] = &CollisionHandler::playerRope;
	m_collisionMap[Key(typeid(Block), typeid(Player))] = &CollisionHandler::blockPlayer; //why do we have this?!?!
	m_collisionMap[Key(typeid(Player), typeid(Block))] = &CollisionHandler::playerBlock;
	m_collisionMap[Key(typeid(CheckPoint), typeid(Player))] = &CollisionHandler::checkPointPlayer;
	m_collisionMap[Key(typeid(Player), typeid(CheckPoint))] = &CollisionHandler::playerCheckPoint;
	m_collisionMap[Key(typeid(Projectile), typeid(Player))] = &CollisionHandler::projectilePlayer;
	m_collisionMap[Key(typeid(Player), typeid(Projectile))] = &CollisionHandler::playerProjectile;
	m_collisionMap[Key(typeid(FallingBlock), typeid(Block))] = &CollisionHandler::fallingBlockBlock;
	m_collisionMap[Key(typeid(Block), typeid(FallingBlock))] = &CollisionHandler::blockFallingBlock;
	m_collisionMap[Key(typeid(Player), typeid(FloorObstacle))] = &CollisionHandler::playerFloorObstacle;
	m_collisionMap[Key(typeid(FloorObstacle), typeid(Player))] = &CollisionHandler::floorObstaclePlayer;

}
void CollisionHandler::playerGift(GameObj* obj1, GameObj* obj2) {
	Player* player = static_cast<Player*> (obj1);
	Gift* gift= static_cast<Gift*> (obj2);

	if (gift->collisionCounter())
		player->collectGift();
	gift->setCollision(true);
	//collect gift
}

void CollisionHandler::playerRope(GameObj* obj1, GameObj* obj2) {
	Player* player = static_cast<Player*> (obj1);
	Rope* rope= static_cast<Rope*> (obj2);
	//std::cout << "player collide with rope\n";
	//if ((sf::Keyboard::isKeyPressed(sf::Keyboard::W)
	//	|| sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
	//	&& !player->getOnRope()) {
	//	player->setOnRope(true);
	//	player->center({rope->getPos().x, player->getPos().y});
	//	return;
	//}
	//if(player->getOnRope())//reached when ends contact with rope
	//	player->setOnRope(false);
	player->center({ rope->getPos().x, player->getPos().y });
	player->toggleCanCatch();
}

void CollisionHandler::playerBlock(GameObj* obj1, GameObj* obj2) {
	Player* player = static_cast<Player*> (obj1);
	Block* block= static_cast<Block*> (obj2);
	//std::cout << "player collide with block\n";
}
void CollisionHandler::playerCheckPoint(GameObj*obj1, GameObj*obj2){
	Player* player = static_cast<Player*> (obj1);
	CheckPoint* checkP = static_cast<CheckPoint*> (obj2);
	if (!checkP->getActive()) {
		if (checkP->getWin()) {
			player->winGame();
		}
		player->setCheckPoint(checkP->getPos());
		checkP->activate();
	}
}

void CollisionHandler::playerProjectile(GameObj* obj1, GameObj* obj2) {
	Player* player = static_cast<Player*> (obj1);
	Projectile* projectile = static_cast<Projectile*> (obj2);
	player->setExternalForce(projectile->getForce(player->getPos()));
}

void CollisionHandler::giftPlayer(GameObj* obj1, GameObj* obj2) {
	playerGift(obj2, obj1);
}

void CollisionHandler::ropePlayer(GameObj* obj1, GameObj* obj2) {
	playerRope(obj2, obj1);
}
void CollisionHandler::blockPlayer(GameObj* obj1, GameObj* obj2) {
	playerBlock(obj2, obj1);
}
void CollisionHandler::checkPointPlayer(GameObj* obj1, GameObj* obj2){
	playerCheckPoint(obj2, obj1);
}

void CollisionHandler::projectilePlayer(GameObj* obj1, GameObj* obj2) {
	playerProjectile(obj2, obj1);
}

void CollisionHandler::fallingBlockBlock(GameObj* obj1, GameObj* obj2) {
	blockFallingBlock(obj2, obj1);
}

void CollisionHandler::blockFallingBlock(GameObj* block, GameObj* fallingBlock)
{
	FallingBlock* fblock = dynamic_cast<FallingBlock*> (fallingBlock);
	if (fblock) {
		fblock->setActiveAnim();
		//std::cout << "Block and falling block collision";
	}
}

void CollisionHandler::playerFloorObstacle(GameObj* player, GameObj* floorObs)
{
	Player* plyer = static_cast<Player*> (player);
	FloorObstacle* florObs = static_cast<FloorObstacle*> (floorObs);
	if (florObs->getActive())
		plyer->setReset(true);
}

void CollisionHandler::floorObstaclePlayer(GameObj* floorObs, GameObj* player)
{
	playerFloorObstacle(player, floorObs);
}

CollisionHandler& CollisionHandler::getRef() {
	static CollisionHandler colision;
	return colision;
}

void CollisionHandler::handleCollision(GameObj* obj1, GameObj* obj2) {
	auto it = m_collisionMap.find(Key(typeid(*obj1), typeid(*obj2)));

	if (it != m_collisionMap.end())
		(this->*(it->second))(obj1, obj2);	
}