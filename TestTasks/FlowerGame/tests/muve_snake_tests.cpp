#pragma once

#include <gtest/gtest.h>

#include "helpers/state_objects_environment.h"

namespace flower::tests
{
    TEST_F( StateObjectsEnvironment, muveSnake ) {

        // the snake moves correctly with the given boundary conditions
        StateObjectsTest stateObjectsTest( 1, 3 );
        stateObjectsTest.emergenceOfNewSnakeByCoordinates( 0, 0 );

        stateObjectsTest.movingSnakeToNextCoordinate();
        stateObjectsTest.movingSnakeToNextCoordinate();

        ASSERT_EQ( stateObjectsTest.getSnakes().begin()->getCoordinateHeadX(), 0 );
        ASSERT_EQ( stateObjectsTest.getSnakes().begin()->getCoordinateHeadY(), 2 );
    }

    TEST_F( StateObjectsEnvironment, ereseSnake ) {

        // the snake is removed if there is no free play
        StateObjectsTest stateObjectsTest( 1, 2 );
        stateObjectsTest.emergenceOfNewSnakeByCoordinates( 0, 0 );

        stateObjectsTest.movingSnakeToNextCoordinate();
        stateObjectsTest.movingSnakeToNextCoordinate();

        ASSERT_EQ( stateObjectsTest.getSnakes().size(), 0 );
    }

    TEST_F( StateObjectsEnvironment, snakeTailMovement ) {

        // the snake's tail moves sequentially behind the head
        StateObjectsTest stateObjectsTest( 1, 3 );
        stateObjectsTest.emergenceOfNewSnakeByCoordinates( 0, 0 );

        stateObjectsTest.movingSnakeToNextCoordinate();

        ASSERT_EQ( stateObjectsTest.getSnakes().begin()->getCoordinateTail()[1].x, 0 );
        ASSERT_EQ( stateObjectsTest.getSnakes().begin()->getCoordinateTail()[1].y, 0 );

        stateObjectsTest.movingSnakeToNextCoordinate();

        ASSERT_EQ( stateObjectsTest.getSnakes().begin()->getCoordinateTail()[1].x, 0 );
        ASSERT_EQ( stateObjectsTest.getSnakes().begin()->getCoordinateTail()[1].y, 1 );
    }
}
