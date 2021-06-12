#include "GameObj.h"
#include "box2d/box2d.h"
#include "Macros.h"
#include "Resources.h"
#include <Animation.h>

GameObj::GameObj(b2World& world, const sf::Vector2f& pos, const sf::Vector2f& size, int bodyType,
	int textureNum) : m_isRemoved(false), m_col(0)
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

void GameObj::updateAnim(float deltaTime){

	//m_sprite.setTextureRect(Animation::getAnimRef().updateAnim(m_row, m_col, deltaTime,m_totalTime, player));

}
