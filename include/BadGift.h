#pragma once
//============================= include section ==============================
#include "Gift.h"
#include <sfml\Graphics.hpp>
//========================== forward declarations ============================
class Controller;
/*============================================================================
 * Class: Bad gift.
 * when bad gifts objects collid with the player it's add new enemy to the
 * level in the location of the door on the map.
 */
class BadGift : public Gift
{
public:
//=================== constractors and destractors section ===================

	BadGift(const sf::Vector2f& = sf::Vector2f(0, 0),
		const sf::Vector2f & = sf::Vector2f(0, 0));
	virtual ~BadGift() = default;
	//=========================== method section =============================

	virtual void handleColision(Controller&) override;
	virtual void handleCollision(MovingObject&, const sf::Vector2f&)override;
private:
};