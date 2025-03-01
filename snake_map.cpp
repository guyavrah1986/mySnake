#include <iostream>
#include <memory>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <utility>
#include <unistd.h>
#include <vector>
#include "macros.h"
#include "snake_map.h"

using namespace std;

SnakeMap::SnakeMap(IN Snake* snake)
{
    this->snake = snake;
    srand(time(NULL));
    update_snake_food(true);
}

SnakeMap::SnakeMap(IN const SnakeMap& other)
{
    this->snake = other.snake;
    this->snake_food = other.snake_food;
    memcpy(this->map_array, other.map_array, MAP_HEIGHT * MAP_WIDTH);
}

SnakeMap::~SnakeMap()
{
    // GuyA: currently, whenever a Snake (of any
    // kind) is "added" to the SnakeMap object, the Snake
    // is allocated on the stack (either main program or in the
    // "helper function") --> so the dtor here should NOT de-allocate
    // it. If this assumption changes, then the dtor MIGHT need to be
    // re-written.
    // 
    // TODO: change it so that the snake object will be allocated
    // autonomously by the SnakeMap "container" class, ideally
    // on the stack of the class, and thus no need for manual
    // deletion (it will imply RAII idiom) and/or it will imply
    // proper encapsulation.
}

/*
This is the "heart" logic of this class. Other than drawing the
map, it also calls several other functions to update it accoriding to
the progress of the game.
*/
void SnakeMap::redraw()
{
    clear_map();
    for (int i = 0; i < MAP_END; i++)
    {
        cout << endl;
    }
    update_score();
    for (int i = 0; i < snake->snake_parts.size(); i++)
    {
        map_array[snake->snake_parts[i].first][snake->snake_parts[i].second] = SNAKE_CHAR;
    }
    update_snake_head();
    update_snake_food(false);
    map_array[snake_food.first][snake_food.second] = SNAKE_FOOD_CHAR;
    for (int i = 0; i < MAP_HEIGHT; i++)
    {
        for (int j = 0; j < MAP_WIDTH; j++)
        {
            cout << map_array[i][j] << " ";
        }
        cout << endl;
    }
}

void SnakeMap::clear_map()
{
    for (int i = 0; i < MAP_HEIGHT; i++)
    {
        for (int j = 0; j < MAP_WIDTH; j++)
        {
            map_array[i][j] = MAP_CHAR;
        }
    }
}

void SnakeMap::update_score()
{
    cout << "Score:" << snake->length << endl;
}

void SnakeMap::update_snake_head()
{
    char snake_head_char = SNAKE_CHAR;
    enum Direction direction = snake->get_direction();
    switch (direction)
    {
    case West:
        snake_head_char = SNAKE_HEAD_WEST;
        break;
    case North:
        snake_head_char = SNAKE_HEAD_NORTH;
        break;
    case East:
        snake_head_char = SNAKE_HEAD_EAST;
        break;
    case South:
        snake_head_char = SNAKE_HEAD_SOUTH;
        break;
    }

    map_array[snake->snake_head.first][snake->snake_head.second] = snake_head_char;
}

void SnakeMap::update_snake_food(IN bool force_update)
{
    if (snake->food_eaten || force_update)
    {
        while (true)
        {
            int random_i = rand() % MAP_WIDTH;
            int random_j = rand() % MAP_HEIGHT;
            if (map_array[random_i][random_j] == MAP_CHAR)
            {
                snake_food = make_pair(random_i, random_j);
                snake->set_snake_food(snake_food);
                snake->food_eaten = false;
                // GuyA: This is where the route to the new food can be
                // calculated casue we know where the new apple will reside
                // at this point of time.
                // NOTES:
                // 1.Was the size of the snake increased at this point?
                // 2.Are we over the "n" limit of ours?
                DisplayPotentialPathToNewApple();
                break;
            }
        }
    }
}

// ===========================================================
// ===========================================================
// GuyA: My additional functions
// ===========================================================
// ===========================================================
void SnakeMap::DisplayPotentialPathToNewApple()
{
    // TODO:
    sleep(1);
    // 0) Copy the current map to a temporary map
    // NOTE: This is an expensive operation, but for
    // start we first wish to have things working
    Snake tmpSnake(*snake);
    SnakeMap tmpSnakeMap(*this);
    
    // GuyA: Just in case, because it was removed from the ctor
    tmpSnakeMap.clear_map();
    cout << "created temporary copies of snake and snake map" << endl;

    // 1) Calculate the path to the new food --> get a list of the 
    // cells the new path will pass through. Make sure food_eaten
    // is set to false
    tmpSnake.food_eaten = false;
    vector<pair<int, int>> potentialPath;
    findPathRecuersively(tmpSnake, tmpSnakeMap, potentialPath);

    // 2) GuyA: TODO - sleep for "a while" (depedns on N and needs to be
    //    fine tuned)
    sleep(1);
}

void SnakeMap::findPathRecuersively(IN Snake& snake, IN SnakeMap& snakeMap, INOUT vector<pair<int, int>>& path)
{
    // Stop conditions:
    // - Food was eaten
    // - The snake head is out of bounds
    // - It became dead (ran into itself)
    if (true == snake.food_eaten)
    {
        // GuyA: Perhapse add a clear_map before and
        // after displaying the "potential solution"
        // TODO: Add some logic to limit the print ONLY for 
        // the first time a path was found
        cout << "APPLE WAS EATEN" << endl;
        displayMapWithPotentialPath(path);
        return;
    }
    
    if (true == is_game_end(&snake))
    {
        cout << "reached end game" << endl;
        return;
    }
    
    // Recursive call to all Direct-ions
    snake.update_movement(West);
    path.emplace_back(snake.snake_head);
    findPathRecuersively(snake, snakeMap, path);
    path.pop_back();

    snake.update_movement(North);
    path.emplace_back(snake.snake_head);
    findPathRecuersively(snake, snakeMap, path);
    path.pop_back();

    snake.update_movement(East);
    path.emplace_back(snake.snake_head);
    findPathRecuersively(snake, snakeMap, path);
    path.pop_back();

    snake.update_movement(South);
    path.emplace_back(snake.snake_head);
    findPathRecuersively(snake, snakeMap, path);
    path.pop_back();
}

void SnakeMap::displayMapWithPotentialPath(IN vector<pair<int, int>>& path) const
{
    for (unsigned char i = 0; i < MAP_HEIGHT; ++i)
    {
        for (unsigned char j = 0; j < MAP_WIDTH; ++j)
        {
            // GuyA: This entire search logic is not optimal
            pair<int, int> currPair(i, j);
            if (true == isItemInVec(currPair, path))
            {
                cout << MAP_GREEN_PATH_CHAR << " ";
            }
            else
            {
                cout << MAP_CHAR << " ";
            }
        }

        cout << endl;
    }
}

// Utils functions:
// ---------------
bool is_game_end(IN Snake* snake)
{
    bool result = false;
    //pair<int, int> snake_head = snake->snake_head;
    if (snake->snake_head.first < 0 || snake->snake_head.first >= MAP_HEIGHT 
        || snake->snake_head.second < 0 || snake->snake_head.second >= MAP_WIDTH)
    {
        result = true;
    }

    if (snake->is_dead)
    {
        result = true;
    }

    return result;
}

bool isItemInVec(IN const pair<int, int>& pairToSearch, IN const vector<pair<int, int>>& vecToSearch)
{
    for (auto it : vecToSearch)
    {
        if (it.first == pairToSearch.first && it.second == pairToSearch.second)
        {
            return true;
        }
    }

    return false;
}