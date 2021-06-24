#pragma once
//============================ include section ===============================
#include <vector>
#include <StaticObj.h>
#include <MovingObj.h>
#include <Macros.h>


class Player ;
class NetworkObject;
class b2World;

class Board {
public:
	//---------Functions----------

	void generateMap(b2World& world);
	void move();
	void draw(sf::RenderWindow& window);
	void updatePhysics(float deltaTime);
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
	//----------Members-----------
	b2World* m_world;
	int m_mapEnum=castle;
	int m_playerId;
	
	//---------Functions----------
	void getValues(std::vector<sf::Vector2f>& vec, std::ifstream& file);
	std::vector<std::unique_ptr<StaticObj>> m_staticObj;
	std::vector<std::unique_ptr<MovingObj>> m_movingObj;
};