#pragma once

namespace snakeGame {

	namespace common {

		enum SizeBoard {
			HEIGHT = 24,
			WIDTH = 24
		};

		enum Direction {
			LEFT,
			RIGHT,
			UP,
			DOWN
		};

		enum Speed {
			HIGH = 100,
			MIDDLE = 200,
			LOW = 300
		};

		enum Color {
			BLUE = 9,
			BIRCH = 11,
			RED = 12,
			YELLOW = 14
		};
	}
}