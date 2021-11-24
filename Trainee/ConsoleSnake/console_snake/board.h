#pragma once

#include <iostream>
#include <memory>
#include <thread>
#include <string>
#include <mutex>
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
		Board(const Board&) = delete;
		Board(const Board&&) = delete;

		Board(common::Coordinate sizeX, common::Coordinate sizeY, common::Speed speed);

		std::shared_ptr<Snake> getSnake();
		bool getGameOverState();

		void join();

	private:

		void direction();
		void boardState();
		void draw();

		std::string m_screenBuffer;
		common::Speed m_speed;
		common::Score m_score;
		common::Coordinate m_sizeX;
		common::Coordinate m_sizeY;
		std::shared_ptr<Snake> m_snake;
		std::unique_ptr<Fruit> m_fruit;
		std::unique_ptr<std::thread> m_inputSignals;
		std::unique_ptr<std::thread> m_renderingBoard;
		std::mutex mtx;
		bool gameOverState;
	};
}