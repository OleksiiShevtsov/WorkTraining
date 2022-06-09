#include "board.h"

snakeGame::Board::Board( common::Coordinate sizeX, common::Coordinate sizeY ) :
	m_sizeX{ sizeX },
	m_sizeY{ sizeY }
{
}

void snakeGame::Board::boardState() {

	if (m_snake->getCoordinateHeadX() == m_fruit->getX() && m_snake->getCoordinateHeadY() == m_fruit->getY()) {
		m_snake->incrementSnake(m_fruit->getX(), m_fruit->getY());
		m_fruit->updateFruitCoordinate();

		switch ( m_speed )
		{
		case common::Speed::HIGH: m_score += 300; break;
		case common::Speed::MIDDLE: m_score += 200; break;
		case common::Speed::LOW: m_score += 100; break;
		}
	}

	m_snake->move();

	if (m_snake->getCoordinateHeadX() <= 0 || m_snake->getCoordinateHeadX() >= m_sizeX - 1 ||
		m_snake->getCoordinateHeadY() <= 0 || m_snake->getCoordinateHeadY() >= m_sizeY - 1) {
		m_gameOverState = true;
	}

	for (int i = 1; i < m_snake->getTail()->size(); ++i) {
		if ((*m_snake->getTail())[0].x == (*m_snake->getTail())[i].x && (*m_snake->getTail())[0].y == (*m_snake->getTail())[i].y) {
			m_gameOverState = true;
		}
	}
}

snakeGame::common::Coordinate snakeGame::Board::getSizeX() const {
	return m_sizeX;
}

snakeGame::common::Coordinate snakeGame::Board::getSizeY() const {
	return m_sizeY;
}
