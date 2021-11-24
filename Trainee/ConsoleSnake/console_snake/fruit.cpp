#include "fruit.h"

snakeGame::Fruit::Fruit(common::Coordinate sizeBoardX, common::Coordinate sizeBoardY) :
	m_sizeBoardX{ sizeBoardX - 3 },
	m_sizeBoardY{ sizeBoardY - 3 }
{
	srand(time(NULL));
	m_x = 2 + rand() % m_sizeBoardX;
	m_y = 2 + rand() % m_sizeBoardY;
}

void snakeGame::Fruit::updateFruitCoordinate() {
	srand(time(NULL));
	m_x = 2 + rand() % m_sizeBoardX;
	m_y = 2 + rand() % m_sizeBoardY;
}

snakeGame::common::Coordinate snakeGame::Fruit::getX() {
	return m_x;
}

snakeGame::common::Coordinate snakeGame::Fruit::getY() {
	return m_y;
}