#include "Board.h"
#include <fstream>
#include <sstream>
#include <Player.h>
#include <Factory.h>
#include <MessagesStructs.h>
#include <NetworkObject.h>
#include "Projectile.h"
#include "box2d/box2d.h"

//-----------------------------------------------------------------------------
/*
input: the box2d world
this function is called after choosing a map, goes to the right file of the map chosen
and generates the info of the level, uses the factory method to do so
*/
void Board::generateMap(b2World& world) {
	m_world = &world;
	m_movingObj.emplace_back(new Player(world,{25.f , 90.f }, PLAYER_SIZE, b2_dynamicBody,m_playerId,*this));
	std::ifstream file;
	std::string fileName = "Level" + std::to_string(m_mapEnum) + ".txt"; 
	file.open("Level" + std::to_string(m_mapEnum) +  + ".txt"); 
	if (!file.is_open()) {
		throw std::out_of_range("Cant open file, for debugging\n");
	}
	char type;
	std::string str;
	while (!file.eof()) {
		file >> type;
		file >> str;
		std::vector<sf::Vector2f> vals; 
		getValues(vals, file);
		if (type == 's')
			m_staticObj.emplace_back(Factory<StaticObj>::create(str, vals,m_mapEnum, world));
		else
			m_movingObj.emplace_back(Factory<MovingObj>::create(str, vals, m_mapEnum, world));

		vals.clear();
	}
	file.close();
}
//-----------------------------------------------------------------------------
/*reads the line in the file and gather the info neededd to create the object
i
*/
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
//-----------------------------------------------------------------------------
//sets the player id
void Board::setId(int id) {
	m_playerId = id;
}
//-----------------------------------------------------------------------------
//move all the moving objects
void Board::move() const{
	for (auto& moving : m_movingObj)
		moving->move();
}
//-----------------------------------------------------------------------------
//draws all objects
void Board::draw(sf::RenderWindow& window) const{
	for(auto& stati :m_staticObj){
		stati->draw(window);
	}
	for (auto& moving : m_movingObj)
		moving->draw(window);
}
//-----------------------------------------------------------------------------
//updates the objects physics and animation based on deltaTime
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
//-----------------------------------------------------------------------------
//sends player reference to the gameState
Player* Board::getPlayerRef() const{
	return static_cast<Player*>(m_movingObj[0].get());
}
//-----------------------------------------------------------------------------
//gets the info so the server can sync all objects between all players 
MovingObjInfo Board::getInfo(unsigned int index) const{
	return m_movingObj[index]->getInfo();
}
//-----------------------------------------------------------------------------
//sets the info the clients got from the server to sync all objects
void Board::setInfo(unsigned int index, const MovingObjInfo& info)  const{
	if(index < m_movingObj.size())
		m_movingObj[index]->setInfo(info);
}
//-----------------------------------------------------------------------------
//returns the size of the moving objects vector
unsigned int Board::numOfMovingObjs() const{
	return m_movingObj.size();
}
//-----------------------------------------------------------------------------
/*
this function gets the network obejct pointer
updates the board each step and checks if we need to
remove any objects
*/
void Board::updateBoard(NetworkObject* netObj) {
	for (int i = 0; i < m_staticObj.size(); ++i) {
		if (netObj && m_staticObj[i]->getCollision()){
				netObj->sendStaticCollision(i);
				m_staticObj[i]->setCollision(false);
		}
		if (m_staticObj[i]->remove()) {
			m_staticObj.erase(m_staticObj.begin() + i);
			--i;
		}
	}
	if (netObj) {
		for (int i = 0; i < m_movingObj.size(); ++i) {
			if (m_movingObj[i]->getCollision()) {
				auto info = m_movingObj[i]->getInfo();
				info.m_index = i;
				netObj->updateSingleMovingObjInfo(info);
			}
		}
	}
}
//-----------------------------------------------------------------------------
void Board::updateStaticMsgCollision(int index) const{
	m_staticObj[index]->MsgCollision();
}
//-----------------------------------------------------------------------------
/*this function is called after a player used a gift in single player, 
or when the server confirms the client that he can shot his projectile
after updating all the other players about it -in multiplayer mode
the function creates the object and shots the projectile
*/
void Board::addProjectile(const struct AddProjectileMessage& info) {
	
	std::unique_ptr<Projectile> proj = std::make_unique<Projectile>(*m_world,
		PROJECTILE_SIZE, b2_dynamicBody, PLAYER_PROJECTILE_DIS,m_mapEnum);
	auto  temp = proj.get();
	proj->setPos(proj->getPosToShotFrom(info.m_to, info.m_frome, info.m_bounds));
		m_movingObj.emplace_back(proj.release());
	temp->shot(info.m_to);
}
