#include <iostream>

#include "testHelperGreenPath.h"
#include "snake_map.h"

using namespace std;

TestHelperGreenPath::TestHelperGreenPath()
{
    // TODO: In case needed, add here "global settings"
    // for all the tests of this class (when it is initialized)
}

TestHelperGreenPath::~TestHelperGreenPath()
{
    // TODO: In case needed, add here "global settings"
    // for all the tests of this class (when it is terminated)
}

int TestHelperGreenPath::TestHelperGreenPathRunTests()
{
    const string funcName = "TestHelperGreenPath::TestHelperGreenPathRunTests - ";
    cout << funcName + "about to start tests" << endl;
    if (0 != testFindPathWhenSnakeExpandToFour())
    {
        return 1;
    }

    // Carry on to next tests (if present)
    return 0;
}
        
int TestHelperGreenPath::testFindPathWhenSnakeExpandToFour()
{
    Snake testSnake;
    SnakeMap snakeMap(&testSnake);

    // Mimic the initial "settings" that should be
    // placed in the SnakeMap.
    snakeMap.redraw();

    // Invoke the "recursive function"
    cout << "about to manually simulate apple being eaten\n\n\n\n" << endl;
    snakeMap.update_snake_food(true);
    return 1;
}

/*
void TestHelperGreenPath::initializeMap(char map[MAP_HEIGHT][MAP_WIDTH])
{
    // Init the entire map
    //clear_map(map);

    // Place the "4 cells length" snake in the map
    // NOTE: The snake head is in the last cell of 
    // the vector.
    const unsigned char snakeLen = 4;
    vector<pair<int, int>> snake_parts(snakeLen);
    snake_parts[0] = move(make_pair<int, int>(3,4));
    snake_parts[1] = move(make_pair<int, int>(3,3));
    snake_parts[2] = move(make_pair<int, int>(3,2));
    snake_parts[3] = move(make_pair<int, int>(3,1));

    // Place the new apple in place
    map[1][2] = SNAKE_FOOD_CHAR;

    // For debug - display the map
    cout << "the map of the test is:" << endl;
    for (unsigned char i = 0; i < MAP_HEIGHT; ++i)
    {
        for (unsigned char j = 0; j < MAP_WIDTH; ++j)
        {
            cout << map[i][j] << " ";
        }
        cout << endl;
    }
}
*/