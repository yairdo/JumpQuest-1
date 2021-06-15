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
void Board::generateMap(b2World& world) {
	/*m_movingObj.resize(10);
	m_staticObj.resize(10);*/
	m_movingObj.emplace_back(new Player(world, { 5.f, 5.f }, { 50.f,50.f }, b2_dynamicBody));
	std::ifstream file;
	file.open("testLevel.txt");
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
			m_staticObj.emplace_back(Factory<StaticObj>::create(str, vals, world));
		else
			m_movingObj.emplace_back(Factory<MovingObj>::create(str, vals, world));


		//auto p = Factory::create(str, vals, world);
	//	m_staticObj.emplace_back(std::move(p));

		//switch (hashIt(str))//b2_staticBody//b2_dynamicBody
		//{
		//case block:
		//	makeObject<Block, StaticObj>(m_staticObj,world,vals[0],vals[1], b2_staticBody);
		//	break;
		//case movingBlock:
		//	makeObject<MovingBlock, MovingObj>(m_movingObj, world, vals[0], vals[1], vals[2], b2_kinematicBody);
		//	break;
		//case gift:
		//	makeObject<Gift, StaticObj>(m_staticObj, world, vals[0], vals[1], b2_staticBody);
		//	break;
		//case shooter:
		//	//makeObject<Shooter,StaticObj>(m_staticObj, world, vals[0], vals[1], b2_staticBody);
		//	break;
		//case fallingBlock:
		//	makeObject<FallingBlock,MovingObj>(m_movingObj, world, vals[0], vals[1], b2_dynamicBody);
		//	break;
		//case floorObs:
		//	//makeObject<FloorObs,MovingObj>(m_movingObj, world, vals[0], vals[1], vals[2], b2_dynamicBody);
		//	break;
		//case rope:
		//	makeObject<Rope, StaticObj>(m_staticObj, world, vals[0], vals[1], b2_staticBody);
		//	break;

		//default:
		//	break;
		//}
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

//TexturesNum Board::hashIt(const std::string& str) {
	//if (str == "Block") return block;
	//if (str == "MovingBlock") return movingBlock;
	//if (str == "Gift") return gift;
	//if (str == "Shooter") return shooter;
	//if (str == "FallingBlock") return fallingBlock;
	//if (str == "FloorObs") return floorObs;
	//if (str == "Rope") return rope;
	//return block;
//}
void Board::move() {
	for (auto& moving : m_movingObj)
		moving->move();
}

void Board::draw(sf::RenderWindow& window) {
	for (auto it = m_staticObj.begin(); it != m_staticObj.end();) {
		if ((*it)->getIsRemoved()) {
			(*it)->destroyBody();
			it = m_staticObj.erase(it);

		}
		else {
			(*it)->draw(window);
			++it;
		}
	}
	for (auto& moving : m_movingObj)
		moving->draw(window);
	//std::cout << "finished drawing Board, for debugging\n";
}

void Board::updatePhysics(float deltaTime) {
	for (auto& moving : m_movingObj) {
		moving->updatePhysics(deltaTime);
	}
}

Player* Board::getPlayerRef() {
	return (Player*)m_movingObj[0].get();
}

sf::Vector2f Board::getLoc(unsigned int index) {
	return m_movingObj[index]->getPos();
}
void Board::setLoc(unsigned int index, sf::Vector2f loc) {
	m_movingObj[index]->setPos(loc);
}

unsigned int Board::numOfMovingObjs() {
	return m_movingObj.size();
}