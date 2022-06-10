#include "board.h"

snakeGame::Board::Board( common::Coordinate sizeX, common::Coordinate sizeY ) :
	m_sizeX{ sizeX },
	m_sizeY{ sizeY },
	m_screenBuffer{ new char[ sizeX * 2 * sizeY + 1 ], 0 }
{
}

snakeGame::common::Coordinate snakeGame::Board::getSizeX() const {
	return m_sizeX;
}

snakeGame::common::Coordinate snakeGame::Board::getSizeY() const {
	return m_sizeY;
}

char* snakeGame::Board::getScreenBuffer() const {
	return m_screenBuffer.m_buffer;
}

void snakeGame::Board::addToBuffer( const char partOfBuffer ) {
	m_screenBuffer.m_buffer[ m_screenBuffer.m_currentPosition ] = partOfBuffer;
	m_screenBuffer.m_buffer[ ++m_screenBuffer.m_currentPosition ] = '\0';
}

void snakeGame::Board::bufferClearing() {
	m_screenBuffer.m_currentPosition = 0;
}
