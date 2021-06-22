#include "Board.h"
#include <fstream>
#include <iostream>
#include<Block.h>
#include <MovingBlock.h>
#include <Gift.h>
#include <Rope.h>
#include <FallingBlock.h>
#include <sstream>
#include <Player.h>
#include <Factory.h>
#include <MessagesStructs.h>
#include <NetworkObject.h>
#include "CheckPoint.h"

void Board::generateMap(b2World& world) {
	m_world = &world;
	/*m_movingObj.resize(10);
	m_staticObj.resize(10);*/
	//changed for map building
	m_movingObj.emplace_back(new Player(world,{25.f , 25.f }, PLAYER_SIZE, b2_dynamicBody,m_playerId,*this));
	std::ifstream file;
	file.open("testLevel.txt");
	//file.open("Level2.txt");
	if (!file.is_open()) {
		std::cout << "cant open file, for debugging\n";
	}
	char type;
	std::string str;
	while (!file.eof()) {
		file >> type;
		file >> str;
		std::vector<sf::Vector2f> vals; //m_staticObj//m_movingObj
		getValues(vals, file);    //StaticObj//MovingObj
		if (type == 's')
			m_staticObj.emplace_back(Factory<StaticObj>::create(str, vals,m_mapEnum, world));
		else
			m_movingObj.emplace_back(Factory<MovingObj>::create(str, vals, m_mapEnum, world));

		vals.clear();
	}
	//std::cout << "finished genereating, for debugging\n";
}

void Board::getValues(std::vector<sf::Vector2f>& vec, std::ifstream& file) {
	std::string temp;
	std::getline(file, temp);
	std::istringstream sstr(temp);
	while (!sstr.eof()) {
		sf::Vector2f t;
		sstr >> t.x >> t.y;
		vec.push_back(t);
	}
}
void Board::setId(int id) {
	m_playerId = id;
}
void Board::move() {
	for (auto& moving : m_movingObj)
		moving->move();
}

void Board::draw(sf::RenderWindow& window) {
	//for (auto it = m_staticObj.begin(); it != m_staticObj.end();) {
	//	if ((*it)->getIsRemoved()) {
	//		(*it)->destroyBody();
	//		it = m_staticObj.erase(it);

	//	}
	//	else {
	//		(*it)->draw(window);
	//		++it;
	//	}
	//}
	
	for(auto& stati :m_staticObj){
		stati->draw(window);
	}
	for (auto& moving : m_movingObj)
		moving->draw(window);
	//std::cout << "finished drawing Board, for debugging\n";
}

void Board::updatePhysics(float deltaTime) {
	for (int i = 0; i < m_movingObj.size(); ++i) {
		m_movingObj[i]->updatePhysics(deltaTime);
		if (m_movingObj[i]->remove()) {
			m_movingObj.erase(m_movingObj.begin() + i);
			--i;
		}
		m_movingObj[i]->updateAnim(deltaTime);
	}
	for (auto& stat : m_staticObj) {
		stat->updateAnim(deltaTime);
	}

}

Player* Board::getPlayerRef() {
	return static_cast<Player*>(m_movingObj[0].get());
}

MovingObjInfo Board::getInfo(unsigned int index) {
	return m_movingObj[index]->getInfo();
}
void Board::setInfo(unsigned int index, const MovingObjInfo& info) {
	m_movingObj[index]->setInfo(info);
	//TEST!!!!!!!!!!!!!!!!!!!!!
	//m_movingObj[index]->fixed(loc);
}

unsigned int Board::numOfMovingObjs() {
	return m_movingObj.size();
}

void Board::updateBoard(NetworkObject* netObj) {
	for (int i = 0; i < m_staticObj.size(); ++i) {
		if (netObj && m_staticObj[i]->getCollision()){
				netObj->sendStaticCollision(i);
				m_staticObj[i]->setCollision(false);
		}
/*		if (m_staticObj[i]->getIsRemoved()) {
			m_staticObj[i]->destroyBody();
			m_staticObj.erase(m_staticObj.begin() + i);
			--i;
		}*/	
		if (m_staticObj[i]->remove()) {
			//m_staticObj[i]->destroyBody();
			m_staticObj.erase(m_staticObj.begin() + i);
			--i;
		}
	}
}

void Board::updateStaticMsgCollision(int index){
	m_staticObj[index]->MsgCollision();
}


void Board::addProjectile(const struct AddProjectileMessage& info) {
	
	std::unique_ptr<Projectile> proj = std::make_unique<Projectile>(*m_world,
		PROJECTILE_SIZE, b2_dynamicBody, PLAYER_PROJECTILE_DIS,m_mapEnum);
	auto  temp = proj.get();
	proj->setPos(proj->getPosToShotFrom(info.m_to, info.m_frome, info.m_bounds));
		m_movingObj.emplace_back(proj.release());
	temp->shot(info.m_to);
	getPlayerRef()->setGotGift(false);
	
}
