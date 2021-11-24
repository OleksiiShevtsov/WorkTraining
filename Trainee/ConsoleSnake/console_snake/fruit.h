#pragma once

#include <cstdlib>
#include <time.h>

#include "type.h"

namespace snakeGame {

	class Fruit {
	public:
		Fruit(common::Coordinate sizeBoardX, common::Coordinate sizeBoardY);

		void updateFruitCoordinate();

		common::Coordinate getX();
		common::Coordinate getY();

	private:
		common::Coordinate m_x;
		common::Coordinate m_y;
		common::Coordinate m_sizeBoardX;
		common::Coordinate m_sizeBoardY;
	};
}