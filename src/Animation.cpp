#include <Animation.h>

//-----------------------------------------------------------------------------
Animation& Animation::getAnimRef()
{
	static Animation anim;
	return anim;
}
//-----------------------------------------------------------------------------
/*
	Function: update animation
	This functions is findig the correct int rect of the texture and return 
	int rect.
	input: row,col,delta time,totla time, type, direction and switch time
	output: the int rectangle
*/
const sf::IntRect Animation::updateAnim(int row, int& col, float deltaTime,
	float& totalTime ,int type,int dir,float switchTime) {
	bool faceLeft = (dir == left) ? true : false;
	int rowCount=findLineLen(type,row);
	setWidthHeight(type);

	
	m_animRect.width = m_width;
	m_animRect.height = m_height;
	
	totalTime += deltaTime;
	if (totalTime >= switchTime) {
		totalTime -= switchTime;
		if(type!=fallingBlock && type!=floorObs)
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

//-----------------------------------------------------------------------------
/*
	Function:find line len
	This function is finding the lenth of the wanted animation row.
	input: type and row
	output: the lenth of the the wanted row
*/
int Animation::findLineLen(int type, int row) {

	switch (type) {
	case player0:
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
	case fallingBlock:
		return FALLING_LEN;
	case checkPoint: 
		return CHECKPOINT_LEN;
	case archer :
		return ARCHER_LEN;
	case arrow:
		return ARROW_LEN;
	case gift:
		return GIFT_LEN;
	case floorObs:
		return FLOOR_OBS_LEN;
	default:
		break;
	}
	return 0;
}
//-----------------------------------------------------------------------------
/*
	Function: set width height
	This function is setting the width and height of the wanted texture.
	input: wanted type
*/
void Animation::setWidthHeight(int type) {
	switch (type) {
	case player0:
		m_width = PLAYER_WIDTH;
		m_height = PLAYER_HEIGHT;
		break;
	case fallingBlock:
		m_width = FALLING_WIDTH;
		m_height = FALLING_HEIGHT;
		break;
	case checkPoint:
		m_width = CHECKPOINT_WIDTH;
		m_height = CHECKPOINT_HEIGHT;
		break;
	case archer:
		m_width = ARCHER_WIDTH;
		m_height = ARCHER_HEIGHT;
		break;
	case arrow:
		m_width = ARROW_WIDTH;
		m_height = ARROW_HEIGHT;
		break;
	case gift:
		m_width = GIFT_WIDTH;
		m_height = GIFT_HEIGHT;
		break;
	case floorObs:
		m_width = FLOOR_OBS_WIDTH;
		m_height = FLOOR_OBS_HEIGHT;
	}
}