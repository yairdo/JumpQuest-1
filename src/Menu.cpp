//#include "Menu.h"
//#include "Macros.h"
//#include "Resources.h"
//Menu::Menu(const sf::Vector2u& windowSize,const sf::Vector2f& titleScale,
//		   const std::vector<int>& vec,int background, int titleInt) :
//	m_middle(windowSize.x / 2.f,windowSize.y / 2.f),
//	m_background(Resources::getResourceRef().
//	getTexture(background)),
//	m_title(Resources::getResourceRef().getTexture(titleInt)){
//	auto x = m_background.getGlobalBounds().width;
//	auto y = m_background.getGlobalBounds().height;
//	m_background.setScale(float(windowSize.x) / x, float(windowSize.y) / y);
//	m_background.setPosition({ 0.f,0.f });
//	m_title.setOrigin(m_title.getGlobalBounds().width / 2,
//		m_title.getGlobalBounds().height / 2);
//	m_title.setPosition(m_middle.x, m_title.getGlobalBounds().height);
//	m_title.setScale(titleScale);
//	//setButtons(windowSize);
//	setButtons(windowSize,vec);
//}
//Menu::Menu(const sf::Vector2u& windowSize) :m_middle(windowSize.x/2.f,
//		windowSize.y/ 2.f), m_background(Resources::getResourceRef().
//		getTexture(menuBackground)),
//		m_title(Resources::getResourceRef().getTexture(title)),
//		m_isMain(true){
//	auto x = m_background.getGlobalBounds().width;
//	auto y = m_background.getGlobalBounds().height;
//	m_background.setScale(float(windowSize.x) / x, float(windowSize.y) / y);
//	m_background.setPosition({ 0.f,0.f });
//	m_title.setOrigin(m_title.getGlobalBounds().width / 2,
//					  m_title.getGlobalBounds().height / 2);
//	m_title.setPosition(m_middle.x, m_title.getGlobalBounds().height );
//	m_title.setScale(1.5f,1.5f);
//	//setButtons(windowSize);
//}

//void Menu::draw(sf::RenderWindow& window) const{
//	window.draw(m_background);
//	window.draw(m_title);
//	std::for_each(m_buttons.begin(), m_buttons.end(), [&](const Button& but)
//		{but.draw(window); });
	/*if (m_isMain)
		std::for_each(m_buttonsMain.begin(), m_buttonsMain.end(), [&](const Button& but)
			{but.draw(window); });
	else
		std::for_each(m_buttonMulti.begin(), m_buttonMulti.end(), [&](const Button& but)
			{but.draw(window); });*/
//}

//void Menu::setTransp(const sf::Vector2f& location) {
//	std::for_each(m_buttons.begin(), m_buttons.end(), [&](Button& but)
//		{but.setTransp(location); });
	/*if (m_isMain)
		std::for_each(m_buttonsMain.begin(),m_buttonsMain.end(),[&](Button& but)
			{but.setTransp(location); });
	else
		std::for_each(m_buttonMulti.begin(), m_buttonMulti.end(), [&](Button& but)
			{but.setTransp(location); });*/
	/*int i = location.y - m_startButPos.y - (m_butHeight / 2);
	i /= (m_butHeight + SPACE_BUTTONS);
	if (i >= 0 && i < m_buttons.size())
		m_buttons[i].setTransp(location);*/

//}
//
//int Menu::checkCollision(const sf::Vector2f& loc) {
//	/*int i = loc.y - m_startButPos.y - m_butHeight / 2;
//	i /= (m_butHeight + SPACE_BUTTONS);
//	if (i >= 0 && i < m_buttons.size())
//		return m_buttons[i].checkCollision(loc);
//	return nullpt;*/
//	for (auto& but : m_buttons) {
//		auto i = but.checkCollision(loc);
//		if (i != nullpt) 
//			return i;
//	}
//	return nullpt;
	/*int butType=nullpt;
	if (m_isMain) {
		for (auto& but : m_buttonsMain) {
			auto i = but.checkCollision(loc);
			if (i != nullpt) {
				butType = i;
				break;
			}
		}
	}
	else {
		for (auto& but : m_buttonMulti) {
			auto i = but.checkCollision(loc);
			if (i != nullpt) {
				butType = i;
				break;
			}
		}

	}
	switch (butType) {
	case multiplayer:
		m_isMain = false;
		break;
	case back:
		m_isMain = true;
		break;
	case host:
		m_isMain = true;
		break;
	case client:
		m_isMain = true;
		break;
	default:
		break;
	}
	return butType;*/
//}

/*void Menu::setButtons(const sf::Vector2u& windowSize) {
	m_startButPos = sf::Vector2f(m_middle.x,m_title.getGlobalBounds().height+ SPACE_BUTTONS*2);
	m_mainButHeigh = (windowSize.y - m_title.getGlobalBounds().height
		- SPACE_BUTTONS * 2 - SPACE_BUTTONS * MAIN_MENU_BUTTONS) / MAIN_MENU_BUTTONS;
	m_multiButHeight = (windowSize.y - m_title.getGlobalBounds().height
		- SPACE_BUTTONS * 2 - SPACE_BUTTONS * MULTI_BUTTONS) / MULTI_BUTTONS;
	auto pos = m_startButPos;
	for (int i = 0; i < MAIN_MENU_BUTTONS; i++) {
		pos.y = m_startButPos.y + i * SPACE_BUTTONS+i* m_mainButHeigh;
		switch (i) {
			case 0 :
				m_buttonsMain.push_back(Button(singlePlayer, pos,
					PIX4LET*strlen("single player"), m_mainButHeigh));
				break;
			case 1 :
				m_buttonsMain.push_back(Button(multiplayer, pos,
					PIX4LET*strlen("multiplayer"), m_mainButHeigh));
				break;
			case 2 :
				m_buttonsMain.push_back(Button(help, pos,
					PIX4LET*strlen("how to play"), m_mainButHeigh));
				break;
			case 3:
				m_buttonsMain.push_back(Button(ext, pos,
					PIX4LET*strlen("exit"), m_mainButHeigh));
				break;
		}
	}
	pos.y = m_startButPos.y;
	for (int i = 0; i < MULTI_BUTTONS; i++) {
		pos.y = m_startButPos.y + i * SPACE_BUTTONS + i * m_multiButHeight;
		switch (i) {
		case 0:
			m_buttonMulti.push_back(Button(host, pos,
				PIX4LET * strlen("host game"), m_mainButHeigh));
			break;
		case 1:
			m_buttonMulti.push_back(Button(client, pos,
				PIX4LET * strlen("join game"), m_mainButHeigh));
			break;
		case 2:
			m_buttonMulti.push_back(Button(back, pos,
				PIX4LET * strlen("back"), m_mainButHeigh));
			break;
		}
	}
}*/
//void Menu::setButtons(const sf::Vector2u& windowSize, const std::vector<int>& vec) {
//	if (vec.size() == 0)
//		throw std::exception("Vector's size cannot be equal to zero\n");
//	m_startButPos = sf::Vector2f(m_middle.x, m_title.getGlobalBounds().height + SPACE_BUTTONS * 2);
//	m_butHeight = (windowSize.y - m_title.getGlobalBounds().height
//		- SPACE_BUTTONS * 2 -SPACE_BUTTONS* vec.size()) / vec.size();
//	auto pos = m_startButPos;
//	for (int i = 0; i < vec.size(); ++i) {
//		pos.y = m_startButPos.y + i * SPACE_BUTTONS + i * m_butHeight;
//		m_buttons.push_back(Button(vec[i], pos, m_butHeight));
//	}
//}