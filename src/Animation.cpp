#include <Animation.h>


//Animation::Animation(){}

Animation& Animation::getAnimRef()
{
	static Animation anim;
	return anim;
}

//Animation::Animation(float width,float height ,sf::Texture& pic,std::vector<short> vec) {
//	;
//}
//types right now : player =1 
const sf::IntRect Animation::updateAnim(int row, int& col, float deltaTime, float& totalTime ,int type
,int dir) {
	bool faceLeft = (dir == left) ? true : false;
	int rowCount=findLineLen(type,row);
	setWidthHeight(type);

	
	m_animRect.width = m_width;
	m_animRect.height = m_height;
	
	totalTime += deltaTime;
	if (totalTime >= SWITCH_TIME) {
		totalTime -= SWITCH_TIME;
		col++;
		if (col >= rowCount) {
			col = 0;
		}
	}
	m_animRect.top = row * m_height;
	if (faceLeft) {
		m_animRect.left = col * m_animRect.width;
		m_animRect.width = abs(m_animRect.width);
	}
	else {
		m_animRect.left = (col+1) * abs(m_animRect.width);
		m_animRect.width = -abs(m_animRect.width);
	}
	return m_animRect;
}

int Animation::findLineLen(int type, int row) {

	switch (type) {
	case player:
		switch (row) {
		case 0:
			return STAND;
		case 1:
			return WALKING;
		case 2:
			return JUMP;
		case 3:
			return CLIMB;
		case 4:
			return STAND;
		}
		break;
	}
	return 0;
}

void Animation::setWidthHeight(int type) {
	switch (type) {
	case player:
		m_width = PLAYER_WIDTH;
		m_height = PLAYER_HEIGHT;
		break;
	}
}