#include "board.h"


snakeGame::Board::Board(common::Coordinate sizeX, common::Coordinate sizeY, common::Speed speed) :
	m_sizeX{ sizeX },
	m_sizeY{ sizeY },
	m_speed{ speed },
	m_score{ 0 },
	m_snake{ std::make_shared<Snake>(m_sizeX / 2, m_sizeY / 2) },
	m_fruit{ std::make_unique<Fruit>(m_sizeX, m_sizeY) },
	gameOverState{ false }
{
	m_inputSignals = std::make_unique<std::thread>(&Board::direction, this);
	m_renderingBoard = std::make_unique<std::thread>(&Board::draw, this);
}

std::shared_ptr<snakeGame::Snake> snakeGame::Board::getSnake() {
	return m_snake;
}

bool snakeGame::Board::getGameOverState() {
	return gameOverState;
}

void snakeGame::Board::join() {
	m_inputSignals->join();
	m_renderingBoard->join();
}

void snakeGame::Board::direction() {
	while (!gameOverState) {

		if (_kbhit()) {
			switch (_getch()) {
			case 75:
				if (m_snake->getDirection() == common::Direction::RIGHT) {
					break;
				}
				m_snake->setDirection(common::Direction::LEFT); break;
			case 77:
				if (m_snake->getDirection() == common::Direction::LEFT) {
					break;
				}
				m_snake->setDirection(common::Direction::RIGHT); break;
			case 72:
				if (m_snake->getDirection() == common::Direction::DOWN) {
					break;
				}
				m_snake->setDirection(common::Direction::UP); break;
			case 80:
				if (m_snake->getDirection() == common::Direction::UP) {
					break;
				}
				m_snake->setDirection(common::Direction::DOWN); break;
			case 27: gameOverState = true; break;
			default: break;
			}
		}

		std::this_thread::sleep_for(std::chrono::milliseconds{ 50 });
	}
}

void snakeGame::Board::boardState() {

	if (m_snake->getCoordinateHeadX() == m_fruit->getX() && m_snake->getCoordinateHeadY() == m_fruit->getY()) {
		m_snake->incrementSnake(m_fruit->getX(), m_fruit->getY());
		m_fruit->updateFruitCoordinate();

		switch (m_speed)
		{
		case common::Speed::HIGH: m_score += 300; break;
		case common::Speed::MIDDLE: m_score += 200; break;
		case common::Speed::LOW: m_score += 100; break;
		}
	}

	m_snake->move();

	if (m_snake->getCoordinateHeadX() <= 0 || m_snake->getCoordinateHeadX() >= m_sizeX - 1 ||
		m_snake->getCoordinateHeadY() <= 0 || m_snake->getCoordinateHeadY() >= m_sizeY - 1) {
		gameOverState = true;
	}

	for (int i = 1; i < m_snake->getTail()->size(); ++i) {
		if ((*m_snake->getTail())[0].x == (*m_snake->getTail())[i].x && (*m_snake->getTail())[0].y == (*m_snake->getTail())[i].y) {
			gameOverState = true;
		}
	}
}

void snakeGame::Board::draw() {

	while (!gameOverState) {

		for (int x = 0; x < m_sizeX; ++x) {
			m_screenBuffer += (char)219;
			m_screenBuffer += (char)219;
		}

		m_screenBuffer += '\n';

		for (int y = 1; y < m_sizeY - 1; ++y) {
			for (int x = 0; x < m_sizeX; ++x) {
				if (x == 0 || x == m_sizeX - 1) {
					m_screenBuffer += (char)219;
					m_screenBuffer += (char)219;
				}
				else if (m_snake->getCoordinateHeadX() == x && m_snake->getCoordinateHeadY() == y) {
					m_screenBuffer += (char)219;
					m_screenBuffer += (char)219;
				}
				else if (m_fruit->getX() == x && m_fruit->getY() == y) {
					m_screenBuffer += "[]";
				}
				else {

					int flag = true;
					for (int i = 1; i < m_snake->getTail()->size(); ++i) {
						if ((m_snake->getTail()->size() > 1) && (*m_snake->getTail())[i].x == x && (*m_snake->getTail())[i].y == y) {
							m_screenBuffer += (char)219;
							m_screenBuffer += (char)219;
							flag = false;
							break;
						}
					}
					if (flag) {
						m_screenBuffer += "  ";
					}
				}
			}

			m_screenBuffer += '\n';
		}

		for (int x = 0; x < m_sizeX; ++x) {
			m_screenBuffer += (char)219;
			m_screenBuffer += (char)219;
		}

		system("cls");

		std::cout << m_screenBuffer << std::endl;
		std::cout << "Score: " << m_score << std::endl;

		m_screenBuffer = "";

		std::this_thread::sleep_for(std::chrono::milliseconds{ m_speed });

		boardState();
	}
}
