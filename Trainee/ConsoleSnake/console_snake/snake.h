#pragma once

#include <vector>
#include <memory>

#include "type.h"
#include "enum.h"

namespace snakeGame {

	class Snake {

		struct Item;
	
	public:

		Snake(common::Coordinate x, common::Coordinate y);

		common::Coordinate getCoordinateHeadX();
		common::Coordinate getCoordinateHeadY();
		std::shared_ptr<std::vector<Item>> getTail();
		common::Direction getDirection();

		void setDirection(common::Direction currentDirection);

		void move();
		void incrementSnake(common::Coordinate x, common::Coordinate y);

	private:

		void movingItem(common::Coordinate x, common::Coordinate y);

		struct Item {
			common::Coordinate x;
			common::Coordinate y;
		};

		std::shared_ptr<std::vector<Item>> m_tail;
		common::Direction m_currentDirection;
	};
}