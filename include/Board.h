#pragma once
//============================ include section ===============================
#include <vector>
#include <StaticObj.h>
#include <MovingObj.h>
#include <Macros.h>
#include "box2d/box2d.h"
class Player ;
class NetworkObject;

class Board {
public:
	/*void generateNewLevel();
	void readFile();*/
	//Board(int );
	void generateMap(b2World& world);
//	TexturesNum hashIt(const std::string& str);
	void move();
	void draw(sf::RenderWindow& window);
	void updatePhysics(float deltaTime);
	//void updateBoard();
	Player* getPlayerRef();
	void setInfo(unsigned int index, const MovingObjInfo& );
	MovingObjInfo getInfo(unsigned int index);
	unsigned int numOfMovingObjs();
	void updateStaticMsgCollision(int index);
	void updateBoard(NetworkObject*);
	int getMap() { return m_mapEnum; }
	void setId(int);
	void setmapEnum(int map=castle) { m_mapEnum=map; }
	void addProjectile(const struct AddProjectileMessage&);

private:
	b2World* m_world;
	int m_mapEnum=castle;
	int m_playerId;
	void getValues(std::vector<sf::Vector2f>& vec, std::ifstream& file);
	//template<class Obj, class Type>
	//void makeObject(std::vector<std::unique_ptr<Type>>& vec,b2World& world,
	//	sf::Vector2f pos, sf::Vector2f size, int bType);

	//template<class Obj, class Type>
	//void makeObject(std::vector<std::unique_ptr<Type>>& vec,b2World& world,sf::Vector2f from ,
	//	sf::Vector2f to,sf::Vector2f size, int bType);
	//std::multimap<float, std::unique_ptr<StaticObj>> m_staticObj;
	std::vector<std::unique_ptr<StaticObj>> m_staticObj;
	std::vector<std::unique_ptr<MovingObj>> m_movingObj;
};

/*
*   block,
	movingBlock,
	shooter,
	fallingRock,
	floorObs
*/
//template<class Obj,class Type>
//void Board::makeObject(std::vector<std::unique_ptr<Type>>& vec,b2World& world,
//	sf::Vector2f pos, sf::Vector2f size, int bType) {
//
//	vec.emplace_back(new Obj(world, pos, size, bType));
//}
//
//template<class Obj, class Type>
//void Board::makeObject(std::vector<std::unique_ptr<Type>>& vec,b2World& world,sf::Vector2f from,
//	sf::Vector2f to, sf::Vector2f size, int bType) {
//
//	vec.emplace_back(new Obj(world, from,to, size, bType));
//}