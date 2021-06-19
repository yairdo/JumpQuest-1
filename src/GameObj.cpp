#include "GameObj.h"
#include "box2d/box2d.h"
#include "Macros.h"
#include "Resources.h"

GameObj::GameObj(b2World& world, const sf::Vector2f& pos, const sf::Vector2f& size, int bodyType,
	int textureNum) : m_isRemoved(false), m_col(0),m_row(0)
{

	m_sprite.setTexture(Resources::getResourceRef().getTexture(textureNum));
	/*m_sprite.setScale(size.x / m_sprite.getGlobalBounds().width, size.y / m_sprite.getGlobalBounds().height);
	m_sprite.setOrigin(m_sprite.getTextureRect().width / 2.f, m_sprite.getTextureRect().height / 2.f);*/
	m_sprite.setPosition(pos);

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

void GameObj::destroyBody(){
	m_body->GetWorld()->DestroyBody(m_body);
}

void GameObj::setPos(sf::Vector2f loc)
{
	m_sprite.setPosition(loc); 
	m_body->SetTransform({ loc.x / SCALE, loc.y / SCALE }, 0);
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

void GameObj::draw(sf::RenderWindow& window) {
	window.draw(m_sprite);
}

