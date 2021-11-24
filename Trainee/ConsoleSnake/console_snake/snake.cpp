#include "snake.h"

snakeGame::Snake::Snake(common::Coordinate x, common::Coordinate y) :
	m_tail{ std::make_shared<std::vector<Item>>() }
{
	m_tail->push_back(Item{ x, y });
}

snakeGame::common::Coordinate snakeGame::Snake::getCoordinateHeadX() {
	return (*m_tail)[0].x;
}

snakeGame::common::Coordinate snakeGame::Snake::getCoordinateHeadY() {
	return (*m_tail)[0].y;
}

std::shared_ptr<std::vector<snakeGame::Snake::Item>> snakeGame::Snake::getTail() {
	return m_tail;
}

snakeGame::common::Direction snakeGame::Snake::getDirection() {
	return m_currentDirection;
}

void snakeGame::Snake::setDirection(common::Direction currentDirection) {
	m_currentDirection = currentDirection;
}

void snakeGame::Snake::move() {
	switch (m_currentDirection)
	{
	case common::Direction::LEFT: movingItem(-1, 0);
		break;
	case common::Direction::RIGHT: movingItem(1, 0);
		break;
	case common::Direction::UP: movingItem(0, 1);
		break;
	case common::Direction::DOWN: movingItem(0, -1);
		break;
	}
}

void snakeGame::Snake::incrementSnake(common::Coordinate x, common::Coordinate y) {
	m_tail->push_back(Item{ x, y });
}

void snakeGame::Snake::movingItem(common::Coordinate x, common::Coordinate y) {

	for (int i = m_tail->size();; --i) {

		if (i == 1) {
			(*m_tail)[0].x += x;
			(*m_tail)[0].y -= y;
			return;
		}
		int flag = true;
		if (i == m_tail->size()) {
			for (int j = 0; j < m_tail->size() - 1; ++j) {
				if ((*m_tail)[m_tail->size() - 1].x == (*m_tail)[j].x &&
					(*m_tail)[m_tail->size() - 1].y == (*m_tail)[j].y) {
					flag = false;
				}
			}
			if (flag) {
				(*m_tail)[i - 1].x = (*m_tail)[i - 2].x;
				(*m_tail)[i - 1].y = (*m_tail)[i - 2].y;
			}
		}
		else {
			(*m_tail)[i - 1].x = (*m_tail)[i - 2].x;
			(*m_tail)[i - 1].y = (*m_tail)[i - 2].y;
		}
	}
}