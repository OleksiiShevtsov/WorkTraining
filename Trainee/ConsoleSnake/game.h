#pragma once

#include <stdlib.h>
#include <thread>

#include "board.h"
#include "snake.h"
#include "fruit.h"

namespace snakeGame {
	class Game{ 
	public:

		Game(common::Speed speed);

		void start();
		void direction();
		void draw();
		void join();

		bool getGameOverState();

	private:

		void boardState();

		common::Speed m_speed;
		common::Score m_score;
		Snake m_snake;
		Fruit m_fruit;
		Board m_board;
		std::unique_ptr<std::thread> m_inputSignals;
		std::unique_ptr<std::thread> m_renderingBoard;
		bool m_gameOverState;
	};
}