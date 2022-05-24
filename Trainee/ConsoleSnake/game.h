#pragma once

#include <stdlib.h>

#include "board.h"

namespace snakeGame {
	class Game{ 
	public:

		Game(common::Speed speed);

		void start();

		std::shared_ptr<Board> getBoard();

	private:
		common::Speed m_speed;
		std::shared_ptr<Board> m_board;
	};
}