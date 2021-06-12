#pragma once
#include <SFML/Graphics.hpp>

class b2Body;
class b2World;

class GameObj {
public:
    GameObj(b2World&, const sf::Vector2f&, const sf::Vector2f&, int, int textureNum);
    virtual ~GameObj() = default;
    virtual void draw(sf::RenderWindow&) = 0;
    virtual void setRemoveObj(bool );
    virtual bool getIsRemoved() const;
    void destroyBody();
    sf::Vector2f getPos() { return m_sprite.getPosition(); }
    //=====temp polichecker
    /*virtual void handleCol(GameObj*) = 0;
    virtual void handleGiftCol(Gift*) = 0;*/
    //virtual void handleGiftCol(Block*) = 0;

    //trying stuff for anime -yair
    void updateAnim();
    int m_row, m_col;
    float m_width=130, m_height=160;
protected:
    b2Body* m_body;
    sf::Sprite m_sprite;

private:
    bool m_isRemoved;
};