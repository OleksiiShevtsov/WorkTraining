#include "game.h"

snakeGame::Game::Game(common::Speed speed) :
	m_speed{ speed }
{
}

void snakeGame::Game::start() {

	m_board = std::make_shared<Board>(24, 24, m_speed);

	m_board->join();
}

std::shared_ptr<snakeGame::Board> snakeGame::Game::getBoard() {
	return m_board;
}