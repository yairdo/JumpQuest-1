#pragma once
#include "MovingObj.h"

class b2World;
class NetworkObject;
class sf::RenderWindow;
class Board;

class Player : public MovingObj {
public:
	Player(b2World&, const sf::Vector2f&, const sf::Vector2f&, int, int , Board& board);
	virtual ~Player()=default;

	void collectGift();
	virtual void updatePhysics(float);
	virtual void move();
	virtual void draw(sf::RenderWindow&)const;
	void useGift(const sf::Vector2f&, NetworkObject*);
	void toggleCanCatch() { m_canCatch = !m_canCatch; };
	void footContact(int val);
	void updateAnim(float deltaTime);
	virtual void reset() override;
	void center(const sf::Vector2f&);
	void winGame();

	//----Setters/getters
	void setOnRope(bool);
	bool getOnRope() const { return m_onRope; };
	void setGotGift(bool state){ m_gotGift = state; }
	bool getGotGift() const { return m_gotGift; }
	void setExternalForce(const b2Vec2&);
	bool getMoving()const { return m_moving; }
	void setMoving(bool vel);
	int getDirection() const;
	void setCheckPoint(const sf::Vector2f&);
	void setName(const std::string&);
	bool getWin() const { return m_win; }
	float getWidth();
	void setPush(bool push) { m_pushDown = push; }

private:
	Board* m_board;
	sf::Text m_name;
	sf::Vector2f m_checkPoint;
	sf::Vector2f m_offSet;
	b2Vec2 m_projectileForce;
	int m_id;
	int m_direction;
	int m_numFootContact;
	float m_stunTime;

	bool m_onRope;
	bool m_canCatch;
	bool m_gotGift;
	bool m_pushDown;
	bool m_win;
	bool m_moving;
	bool m_stuned;

	void ropeJump(float, int);
	void applyProjectileImpulse();
	bool horizontalMove(int, bool&, int&, float, int);
	void velocityCorrection(bool, b2Vec2);
	void updateAnimPos(int, int);
	void updateRow();
	void jump(float);
};