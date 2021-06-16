#include <iostream>
#include "CollisionHandler.h"
#include "Gift.h"
#include "Player.h"
#include "Rope.h"
#include "Block.h"
#include "FallingBlock.h"
#include <iostream>
#include <functional>
#include <Macros.h>
CollisionHandler::CollisionHandler(){
	/*std::function<void(GameObj*, GameObj*)> Func=giftPlayer;
	m_collisionMap[Key(typeid(Gift), typeid(Player))] = &[]() {return &giftPlayer; };*/
	/*auto p = Key(typeid(Gift), typeid(Player));*/
	m_collisionMap[Key(typeid(Gift), typeid(Player))] = &CollisionHandler::giftPlayer;
	m_collisionMap[Key(typeid(Player), typeid(Gift))] = &CollisionHandler::playerGift;
	m_collisionMap[Key(typeid(Rope), typeid(Player))] = &CollisionHandler::ropePlayer;
	m_collisionMap[Key(typeid(Player), typeid(Rope))] = &CollisionHandler::playerRope;
	m_collisionMap[Key(typeid(Block), typeid(Player))] = &CollisionHandler::blockPlayer;
	m_collisionMap[Key(typeid(Player), typeid(Block))] = &CollisionHandler::playerBlock;

}
void CollisionHandler::playerGift(GameObj* obj1, GameObj* obj2) {
	Player* player = static_cast<Player*> (obj1);
	Gift* gift= static_cast<Gift*> (obj2);
	gift->collisionCounter();
	gift->setCollision(true);
	//collect gift
}

void CollisionHandler::playerRope(GameObj* obj1, GameObj* obj2) {
	Player* player = static_cast<Player*> (obj1);
	Rope* rope= static_cast<Rope*> (obj2);
	//std::cout << "player collide with rope\n";
	if ((sf::Keyboard::isKeyPressed(sf::Keyboard::W) 
		|| sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) 
		&& !player->getOnRope()) {
		player->setOnRope(true);
		return;
	}
	if(player->getOnRope())//reached when ends contact with rope
		player->setOnRope(false);

}

void CollisionHandler::playerBlock(GameObj* obj1, GameObj* obj2) {
	Player* player = static_cast<Player*> (obj1);
	Block* block= static_cast<Block*> (obj2);
	//std::cout << "player collide with block\n";
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
CollisionHandler& CollisionHandler::getRef() {
	static CollisionHandler colision;
	return colision;
}

void CollisionHandler::handleCollision(GameObj* obj1, GameObj* obj2) {
	auto it = m_collisionMap.find(Key(typeid(*obj1), typeid(*obj2)));
	//std::cout << typeid(*obj1).name() << " " << typeid(*obj2).name() << std::endl;
	//auto& func = it->second;
	//func(obj1, obj2);
	//(this->func)(obj1, obj2);
	//(this->(it->second))(obj1, obj2);
	//this->(it->second(obj1, obj2));

	if (it != m_collisionMap.end())
		(this->*(it->second))(obj1, obj2);
		//(*(it->second))(obj1, obj2);
	
}