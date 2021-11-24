#pragma once

#include <iostream>

#include "game.h"

namespace snakeGame {
	class Menu {

	public:

		Menu();
		void start();

	private:
		
		void extend();
		void colorSelection();
		void speedSelection();

		common::Speed m_speed;
		std::unique_ptr<Game> m_game;
		HANDLE m_consoleColour;
	};
}