#include "board.h"

snakeGame::Board::Board(common::Coordinate sizeX, common::Coordinate sizeY) :
	m_sizeX{ sizeX },
	m_sizeY{ sizeY },
	m_screenBuffer{ "" }
{
}

snakeGame::common::Coordinate snakeGame::Board::getSizeX() const {
	return m_sizeX;
}

snakeGame::common::Coordinate snakeGame::Board::getSizeY() const {
	return m_sizeY;
}

std::string snakeGame::Board::getScreenBuffer() const {
	return m_screenBuffer;
}

void snakeGame::Board::addToBuffer( const char partOfBuffer ) {
	m_screenBuffer = m_screenBuffer + partOfBuffer;
}

void snakeGame::Board::bufferClearing() {
	m_screenBuffer = "";
}
