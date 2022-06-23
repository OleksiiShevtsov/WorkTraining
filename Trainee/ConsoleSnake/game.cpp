#include "game.h"

snakeGame::Game::Game(common::Speed speed) :
	m_speed{ speed }, 
	m_score { 0 },
	m_snake{ common::SizeBoard::WIDTH / 2, common::SizeBoard::HEIGHT / 2 },
	m_fruit{ common::SizeBoard::WIDTH, common::SizeBoard::HEIGHT },
	m_board{ common::SizeBoard::WIDTH, common::SizeBoard::HEIGHT },
	m_gameOverState{ false },
	m_closingIncomDirection{ false }
{
}

void snakeGame::Game::start() {

	m_renderingBoard = std::make_unique< std::thread >( &Game::draw, this );
    m_inputSignals = std::make_unique< std::thread >( &Game::direction, this );
}

void snakeGame::Game::direction() {
	while ( !m_gameOverState ) {

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
	while ( !m_gameOverState ) {

		for ( int x = 0; x < m_board.getSizeX(); ++x ) {
			m_board.addToBuffer( (char)219 );
			m_board.addToBuffer( (char)219 );
		}

		m_board.addToBuffer( '\n' );

		for ( int y = 1; y < m_board.getSizeY() - 1; ++y ) {
			for ( int x = 0; x < m_board.getSizeX(); ++x ) {
				if ( x == 0 || x == m_board.getSizeX() - 1 ) {
					m_board.addToBuffer( (char)219 );
					m_board.addToBuffer( (char)219 );
				}
				else if ( m_snake.getCoordinateHeadX() == x && m_snake.getCoordinateHeadY() == y ) {
					m_board.addToBuffer( (char)219 );
					m_board.addToBuffer( (char)219 );
				}
				else if (m_fruit.getX() == x && m_fruit.getY() == y) {
					m_board.addToBuffer( '[' );
					m_board.addToBuffer( ']' );
				}
				else {

					int flag = true;
					for (int i = 1; i < m_snake.getTail().size(); ++i) {
						if ( m_snake.getTail().size() > 1 && m_snake.getTail()[ i ].x == x && m_snake.getTail()[ i ].y == y ) {
							m_board.addToBuffer( (char)219 );
							m_board.addToBuffer( (char)219 );
							flag = false;
							break;
						}
					}
					if (flag) {
						m_board.addToBuffer( ' ' );
						m_board.addToBuffer( ' ' );
					}
				}
			}

			m_board.addToBuffer( '\n' );
		}

		for (int x = 0; x < m_board.getSizeX(); ++x) {
			m_board.addToBuffer( (char)219 );
			m_board.addToBuffer( (char)219 );
		}

		system( "cls" );

		std::cout << m_board.getScreenBuffer() << std::endl;
		std::cout << "Score: " << m_score << std::endl;

		m_board.bufferClearing();

		std::this_thread::sleep_for(std::chrono::milliseconds{ m_speed });

		boardState();
	}
}

void snakeGame::Game::boardState() {

	if ( m_snake.getCoordinateHeadX() == m_fruit.getX() && m_snake.getCoordinateHeadY() == m_fruit.getY() ) {
		m_snake.incrementSnake( m_fruit.getX(), m_fruit.getY() );
		m_fruit.updateFruitCoordinate();

		switch ( m_speed )
		{
			case common::Speed::HIGH: m_score += 300; break;
			case common::Speed::MIDDLE: m_score += 200; break;
			case common::Speed::LOW: m_score += 100; break;
		}
	}

	m_snake.move();

	if ( m_snake.getCoordinateHeadX() <= 0 || m_snake.getCoordinateHeadX() >= m_board.getSizeX() - 1 ||
		m_snake.getCoordinateHeadY() <= 0 || m_snake.getCoordinateHeadY() >= m_board.getSizeY() - 1 ) {
		m_gameOverState = true;
		return;
	}

	for ( int i = 1; i < m_snake.getTail().size(); ++i ) {
		if ( m_snake.getTail()[ 0 ].x == m_snake.getTail()[ i ].x && m_snake.getTail()[ 0 ].y == m_snake.getTail()[ i ].y ) {
			m_gameOverState = true;
			return;
		}
	}
}

void snakeGame::Game::join() {
	m_renderingBoard->join();
	m_inputSignals->join();
}
