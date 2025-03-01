#include <algorithm>
#include <bits/stdc++.h>
#include <cstdio>
#include <stdio.h>
#include <iostream>
#include <string>
#include <unistd.h>
#include <utility>
#include "input.h"
#include "snake_map.h"
#include "testHelperGreenPath.h"

using namespace std;

// GuyA: TODO - this is NOT optimal. Need to encapsulate it.
SnakeThread snake;
SnakeMap snake_map(&snake);

void initialize()
{
    input_enter_off();
}

void game_over()
{
    cout << "GAME IS OVER" << endl;
}

void start_game()
{
    enum Direction dummyDirection = East;
    while (true)
    {
        // GuyA: Having the below function reciving an
        // argument that it does need, is NOT good, but
        // for now it is needed...
        snake.update_movement(dummyDirection);
        if (is_game_end(&snake))
        {
            game_over();
            break;
        }
        snake_map.redraw();

        usleep(PAUSE_LENGTH);

        snake.validate_direction();
    }
}

/*
This function parses the command line arguments
and check what should be run accordingly.
Returns: 0 - normal excution (no arguments providied)
         1 - unit tests (-t was handed)
*/
int parseCommandLineArgs(IN int argc, IN char** argv)
{
    // If no arguments are provided, return 0 for normal execution
    if (1 == argc)
    {
        cout << "normal execution" << endl;
        return 0;
    }

    if (2 == argc)
    {
        if (strcmp(argv[1], "-t") == 0)
        {
            return 1; // Unit tests should be run
        }
    }

    // If no recognized argument is found, return normal execution
    return 0;
}

int main(int argc, char** argv)
{
    if (1 == parseCommandLineArgs(argc, argv))
    {
        TestHelperGreenPath t;
        if (0 == t.TestHelperGreenPathRunTests())
        {
            cout << "all tests PASSED" << endl;
            return 0;
        }

        cout << "one (or more) tests FAILED" << endl;
        return 1;
    }

    initialize();
    start_game();
    return 0;
}
