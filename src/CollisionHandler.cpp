#include <iostream>
#include "CollisionHandler.h"
#include "Gift.h"
#include "Player.h"
#include "Rope.h"
#include "MovingBlock.h"
#include "Block.h"
#include "CheckPoint.h"
#include "FallingBlock.h"
#include "FloorObstacle.h"
#include <Projectile.h>


//-----------------------------------------------------------------------------
//CollisionHnadle c-tor , builds the map for the multimethods
CollisionHandler::CollisionHandler(){

	m_collisionMap[Key(typeid(Gift), typeid(Player))] = &CollisionHandler::giftPlayer;
	m_collisionMap[Key(typeid(Player), typeid(Gift))] = &CollisionHandler::playerGift;
	m_collisionMap[Key(typeid(Rope), typeid(Player))] = &CollisionHandler::ropePlayer;
	m_collisionMap[Key(typeid(Player), typeid(Rope))] = &CollisionHandler::playerRope;
	m_collisionMap[Key(typeid(MovingBlock), typeid(Player))] = &CollisionHandler::movingBlockPlayer;
	m_collisionMap[Key(typeid(Player), typeid(MovingBlock))] = &CollisionHandler::playerMovingBlock;
	m_collisionMap[Key(typeid(CheckPoint), typeid(Player))] = &CollisionHandler::checkPointPlayer;
	m_collisionMap[Key(typeid(Player), typeid(CheckPoint))] = &CollisionHandler::playerCheckPoint;
	m_collisionMap[Key(typeid(FallingBlock), typeid(Block))] = &CollisionHandler::fallingBlockBlock;
	m_collisionMap[Key(typeid(Block), typeid(FallingBlock))] = &CollisionHandler::blockFallingBlock;
	m_collisionMap[Key(typeid(Player), typeid(FloorObstacle))] = &CollisionHandler::playerFloorObstacle;
	m_collisionMap[Key(typeid(FloorObstacle), typeid(Player))] = &CollisionHandler::floorObstaclePlayer;
	m_collisionMap[Key(typeid(Projectile), typeid(Player))] = &CollisionHandler::projectilePlayer;
	m_collisionMap[Key(typeid(Player), typeid(Projectile))] = &CollisionHandler::playerProjectile;
	//--
	m_collisionMap[Key(typeid(FallingBlock), typeid(Player))] = &CollisionHandler::fallingBlockPlayer;
	m_collisionMap[Key(typeid(Player), typeid(FallingBlock))] = &CollisionHandler::playerFallingBlock;


}
//-----------------------------------------------------------------------------
//when player collides with gift
void CollisionHandler::playerGift(GameObj* obj1, GameObj* obj2) {
	Player* player = static_cast<Player*> (obj1);
	Gift* gift= static_cast<Gift*> (obj2);

	if (gift->collisionCounter())
		player->collectGift();
	gift->setCollision(true);
}
//-----------------------------------------------------------------------------
//when player collides with rope
void CollisionHandler::playerRope(GameObj* obj1, GameObj* obj2) {
	Player* player = static_cast<Player*> (obj1);
	Rope* rope= static_cast<Rope*> (obj2);
	player->center({ rope->getPos().x, player->getPos().y });
	player->toggleCanCatch();
}
//-----------------------------------------------------------------------------
//when player collides with movingBlock
void CollisionHandler::playerMovingBlock(GameObj* obj1, GameObj* obj2) {
	Player* player = static_cast<Player*> (obj1);
	MovingBlock* mvBlock= static_cast<MovingBlock*> (obj2);
	if (!player->getMoving()) {
		player->setMoving(true);
		if ((abs(mvBlock->getPos().x - player->getPos().x)) >= (mvBlock->getWidth() / 2) + (player->getWidth() / 4)+1)
			player->setPush(true);
		else
			player->setPush(false);
	}
	else {
		player->setMoving(false);
	}
}
//-----------------------------------------------------------------------------
//when player collides with CheckPoint
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
//-----------------------------------------------------------------------------
//when player collides with Projectile
void CollisionHandler::playerProjectile(GameObj* obj1, GameObj* obj2) {
	Player* player = static_cast<Player*> (obj1);
	Projectile* projectile = static_cast<Projectile*> (obj2);
	player->setExternalForce(projectile->getForce(player->getPos()));
}
//-----------------------------------------------------------------------------
//when player collides with gift
void CollisionHandler::giftPlayer(GameObj* obj1, GameObj* obj2) {
	playerGift(obj2, obj1);
}
//-----------------------------------------------------------------------------
//when player collides with rope
void CollisionHandler::ropePlayer(GameObj* obj1, GameObj* obj2) {
	playerRope(obj2, obj1);
}
//-----------------------------------------------------------------------------
//when player collides with movingBlock
void CollisionHandler::movingBlockPlayer(GameObj* obj1, GameObj* obj2) {
	playerMovingBlock(obj2, obj1);
}
//-----------------------------------------------------------------------------
//when player collides with CheckPoint
void CollisionHandler::checkPointPlayer(GameObj* obj1, GameObj* obj2){
	playerCheckPoint(obj2, obj1);
}
//-----------------------------------------------------------------------------
//when player collides with Projectiile
void CollisionHandler::projectilePlayer(GameObj* obj1, GameObj* obj2) {
	playerProjectile(obj2, obj1);
}
//-----------------------------------------------------------------------------
//when Block collides with fallingBlock
void CollisionHandler::fallingBlockBlock(GameObj* obj1, GameObj* obj2) {
	blockFallingBlock(obj2, obj1);
}
//-----------------------------------------------------------------------------
//when player collides with fallingBlock
void CollisionHandler::fallingBlockPlayer(GameObj* obj1, GameObj* obj2) {
	playerFallingBlock(obj2, obj1);
}
//-----------------------------------------------------------------------------
//when player collides with falling block
void CollisionHandler::playerFallingBlock(GameObj* obj1, GameObj* obj2) {
	FallingBlock* fBlock = static_cast<FallingBlock*> (obj2);
	if(!fBlock->getCollision())
		fBlock->setCollision(true);
	else
		fBlock->setCollision(false);
}
//-----------------------------------------------------------------------------
//when Block collides with FallingBlock
void CollisionHandler::blockFallingBlock(GameObj* block, GameObj* fallingBlock)
{
	FallingBlock* fblock = static_cast<FallingBlock*> (fallingBlock);
	fblock->setActiveAnim();
}
//-----------------------------------------------------------------------------
//when player collides with FloorObstacle
void CollisionHandler::playerFloorObstacle(GameObj* player, GameObj* floorObs)
{
	Player* plyer = static_cast<Player*> (player);
	FloorObstacle* florObs = static_cast<FloorObstacle*> (floorObs);
	if (florObs->getActive())
		plyer->setReset(true);
}
//-----------------------------------------------------------------------------
//when player collides with FloorObstacle
void CollisionHandler::floorObstaclePlayer(GameObj* floorObs, GameObj* player)
{
	playerFloorObstacle(player, floorObs);
}
//-----------------------------------------------------------------------------
//gets the reference for the singeltone collision map
CollisionHandler& CollisionHandler::getRef() {
	static CollisionHandler colision;
	return colision;
}
//-----------------------------------------------------------------------------
//handles the collision between 2 objects
void CollisionHandler::handleCollision(GameObj* obj1, GameObj* obj2) {
	auto it = m_collisionMap.find(Key(typeid(*obj1), typeid(*obj2)));

	if (it != m_collisionMap.end())
		(this->*(it->second))(obj1, obj2);	
}