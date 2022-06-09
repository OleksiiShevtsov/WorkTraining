#pragma once

#include <iostream>
#include <memory>
#include <thread>
#include <string>
#include <chrono>
#include <conio.h>

#include <windows.h>

#include "enum.h"
#include "snake.h"
#include "fruit.h"

namespace snakeGame {
	class Board {
	public:

		Board() = delete;
		Board( const Board& ) = delete;
		Board( const Board&& ) = delete;

		Board( common::Coordinate sizeX, common::Coordinate sizeY );

		void boardState();

		common::Coordinate getSizeX() const;
		common::Coordinate getSizeY() const;

	private:

		std::string m_screenBuffer;
		common::Coordinate m_sizeX;
		common::Coordinate m_sizeY;
	};
}