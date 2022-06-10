#pragma once

#include <iostream>
#include <memory>
#include <string>
#include <conio.h>

#include <windows.h>

#include "enum.h"
#include "type.h"

namespace snakeGame {
	class Board {
	public:

		Board() = delete;
		Board( const Board& ) = delete;
		Board( const Board&& ) = delete;

		Board( common::Coordinate sizeX, common::Coordinate sizeY );

		common::Coordinate getSizeX() const;
		common::Coordinate getSizeY() const;
		char* getScreenBuffer() const;

		void addToBuffer( const char* partOfBuffer );
		void addToBuffer( const char partOfBuffer );
		void bufferClearing();
		
	private:

		struct ScreenBuffer
		{
			char* m_buffer;
			int m_currentPosition;
		};

		ScreenBuffer m_screenBuffer;

		common::Coordinate m_sizeX;
		common::Coordinate m_sizeY;
	};
}