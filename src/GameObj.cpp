#include "GameObj.h"
#include "box2d/box2d.h"
#include "Macros.h"
#include "Resources.h"
#include <iostream>
GameObj::GameObj(b2World& world, const sf::Vector2f& pos, const sf::Vector2f& size, int bodyType,
	float recWidth, float recHeight,int textureNum,int mapEnum) : m_isRemoved(false), m_col(0),m_row(0),
	m_sprite(Resources::getResourceRef().getTexture(mapEnum, textureNum)), m_collision(false)
{
	//if (textureNum!=rope)
	setSprite(recWidth, recHeight, size, pos);
	//m_sprite.setTexture(Resources::getResourceRef().getTexture(mapEnum,textureNum));
	//std::cout << mapEnum<<"\n";
	/*m_sprite.setScale(size.x / m_sprite.getGlobalBounds().width, size.y / m_sprite.getGlobalBounds().height);
	m_sprite.setOrigin(m_sprite.getTextureRect().width / 2.f, m_sprite.getTextureRect().height / 2.f);*/
	//m_sprite.setPosition(pos);

    b2BodyDef bodyDef;
    bodyDef.type = (b2BodyType)bodyType;
    bodyDef.position.Set(pos.x / SCALE, pos.y / SCALE);
    m_body = world.CreateBody(&bodyDef);
}

void GameObj::setRemoveObj(bool removed){
	m_isRemoved=removed;
}

bool GameObj::getIsRemoved() const{
	return m_isRemoved;
}

void GameObj::setSprite(float recWidth, float recHeight, const sf::Vector2f& size, const sf::Vector2f& pos)
{
	if (recWidth!=0 && recHeight!=0)
		m_sprite.setTextureRect(sf::IntRect(0, 0, recWidth, recHeight));
	m_sprite.setScale(size.x / m_sprite.getGlobalBounds().width, size.y / m_sprite.getGlobalBounds().height);
	m_sprite.setOrigin(m_sprite.getTextureRect().width / 2.f, m_sprite.getTextureRect().height / 2.f);
	m_sprite.setPosition(pos);
	
}

void GameObj::destroyBody(){
	m_body->GetWorld()->DestroyBody(m_body);
}

void GameObj::setPos(sf::Vector2f loc)
{
	m_sprite.setPosition(loc); 
	m_body->SetTransform({ loc.x / SCALE, loc.y / SCALE }, 0);
}

bool GameObj::remove(){
	if (getIsRemoved()) {
		m_body->GetWorld()->DestroyBody(m_body);
		return true;
	}
	return false;
}

void GameObj::updateAnim(float deltaTime){

	//m_sprite.setTextureRect(Animation::getAnimRef().updateAnim(m_row, m_col, deltaTime,m_totalTime, player));

}

b2Fixture* GameObj::createFixtureDef( b2PolygonShape& shape, float density, float friction, uint16 categoryBits, bool isSensor, uint16 maskBits)
{
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &shape;
	fixtureDef.density = density;
	fixtureDef.friction = friction;
	fixtureDef.restitution = 0;
	fixtureDef.filter.categoryBits = categoryBits;
	fixtureDef.isSensor = isSensor;
	fixtureDef.filter.maskBits = maskBits;
	return m_body->CreateFixture(&fixtureDef);
	
}

b2PolygonShape GameObj::createPolygonShape(const sf::Vector2f& halfSize)
{
	b2PolygonShape dynamicBox;
	dynamicBox.SetAsBox(halfSize.x, halfSize.y);
	return std::move(dynamicBox);
}

void GameObj::draw(sf::RenderWindow& window) const{
	window.draw(m_sprite);
}

