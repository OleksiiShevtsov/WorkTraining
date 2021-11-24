#include "menu.h"

snakeGame::Menu::Menu() :
	m_consoleColour{ GetStdHandle(STD_OUTPUT_HANDLE) }
{
}

void snakeGame::Menu::start() {

	std::cout << "welcome to the snake game!" << std::endl;

	extend();

	while (true) {
		std::cout << "Memu:" << std::endl;
		colorSelection();
		speedSelection();
		m_game = std::make_unique<Game>(m_speed);
		m_game->start();
		m_game.reset();
		extend();
	}
}

void snakeGame::Menu::extend() {
	std::cout << std::endl;
	std::cout << "enter any letter and press ENTER: ";
	int extend;
	std::cin >> extend;
	system("cls");
}

void snakeGame::Menu::colorSelection() {
	std::cout << "select color: 1 - BLUE, 2 - RED, 3 - YELLOW, other - leave current : ";
	int color;
	std::cin >> color;
	switch (color)
	{
	case 1: SetConsoleTextAttribute(m_consoleColour, common::BLUE); return;
	case 2: SetConsoleTextAttribute(m_consoleColour, common::RED); return;
	case 3: SetConsoleTextAttribute(m_consoleColour, common::YELLOW); return;
	default: return;
	}
}

void snakeGame::Menu::speedSelection() {
	while (true) {
		std::cout << "select speed: 1 - LOW, 2 - MIDDLE, 3 - HIGH: ";
		int speed;
		std::cin >> speed;
		switch (speed)
		{
		case 1: m_speed = common::LOW; return;
		case 2: m_speed = common::MIDDLE; return;
		case 3: m_speed = common::HIGH; return;
		default: std::cout << "there is no such option, repeat: " << std::endl; break;
		}
	}
}