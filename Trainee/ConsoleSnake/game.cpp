#include "game.h"

snakeGame::Game::Game(common::Speed speed) :
	m_speed{ speed }, 
	m_score { 0 },
	m_snake{ common::SizeBoard::WIDTH / 2, common::SizeBoard::HEIGHT / 2 },
	m_fruit{ common::SizeBoard::WIDTH, common::SizeBoard::HEIGHT },
	m_board{ common::SizeBoard::WIDTH, common::SizeBoard::HEIGHT },
	m_gameOverState{ false }
{
}

void snakeGame::Game::start() {

	m_inputSignals = std::make_unique<std::thread>(&direction, this);
	m_renderingBoard = std::make_unique<std::thread>(&draw, this);

	join();
}

void snakeGame::Game::direction() {
	while (!m_gameOverState) {

		if (_kbhit()) {
			switch (_getch()) {
			case 75:
				if (m_snake.getDirection() == common::Direction::RIGHT) {
					break;
				}
				m_snake.setDirection(common::Direction::LEFT); break;
			case 77:
				if (m_snake.getDirection() == common::Direction::LEFT) {
					break;
				}
				m_snake.setDirection(common::Direction::RIGHT); break;
			case 72:
				if (m_snake.getDirection() == common::Direction::DOWN) {
					break;
				}
				m_snake.setDirection(common::Direction::UP); break;
			case 80:
				if (m_snake.getDirection() == common::Direction::UP) {
					break;
				}
				m_snake.setDirection(common::Direction::DOWN); break;
			case 27: m_gameOverState = true; break;
			default: break;
			}
		}

		std::this_thread::sleep_for( std::chrono::milliseconds{ 100 } );
	}
}

void snakeGame::Game::draw() {

	while (!m_gameOverState) {

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

		m_board.boardState();
	}
}

void snakeGame::Game::join() {
	m_inputSignals->join();
	m_renderingBoard->join();
}
