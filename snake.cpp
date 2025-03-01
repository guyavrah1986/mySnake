#include <iostream>
#include <pthread.h>
#include <string.h>
#include <utility>
#include <vector>
#include "snake.h"
#include "snake_map.h"

using namespace std;

/*
This is the "worker" function of the Snake object. It sole
purpose is to intercpet the key from the user (that indicates
to which direction the snake should turn).
*/
void *input_thread_work(IN void *arg)
{
    struct Snake *snake = (struct Snake *)arg;
    while (true)
    {
        enum Direction direction = get_input();
        snake->update_next_direction(direction);
    }
}

Snake::Snake()
{
    direction = East;
    next_direction = direction;
    food_eaten = false;
    is_dead = false;
    length = INITIAL_SNAKE_LENGTH;
    initialize_snake();
}

Snake::~Snake()
{

}

Snake::Snake(IN const Snake& other)
{
    this->direction = other.direction;
    this->next_direction = other.next_direction;
    this->food_eaten = other.food_eaten;
    this->is_dead = other.is_dead;
    // GuyA: TODO - is it indeed correct (the proper length)
    // at the point where the copy is made?
    this->length = other.length;
    this->snake_parts = other.snake_parts;
    this->snake_food = other.snake_food;
    this->snake_head = other.snake_head;
    memcpy(this->snake_world_array, other.snake_world_array, MAP_HEIGHT * MAP_WIDTH);
}

/*
This function sets the snake object with initial "settings"
such as where it is located at the begining of the game.
*/
void Snake::initialize_snake()
{
    for (int i = 0; i < INITIAL_SNAKE_LENGTH; i++)
    {
        pair<int, int> snake_part = make_pair(MAP_HEIGHT / 2, MAP_WIDTH / 2 - (INITIAL_SNAKE_LENGTH / 2) + i);
        snake_parts.push_back(snake_part);
        snake_world_array[snake_part.first][snake_part.second] = 1;
    }
    snake_head = snake_parts[snake_parts.size() - 1];
}

void Snake::update_direction(IN enum Direction direction)
{
    switch (direction)
    {
    case West:
        if (this->direction != East)
        {
            this->direction = direction;
        }
        break;
    case North:
        if (this->direction != South)
        {
            this->direction = direction;
        }
        break;
    case East:
        if (this->direction != West)
        {
            this->direction = direction;
        }
        break;
    case South:
        if (this->direction != North)
        {
            this->direction = direction;
        }
        break;
    }
}

void Snake::update_next_direction(IN enum Direction direction)
{
    this->next_direction = direction;
}

enum Direction Snake::get_direction()
{
    // Not really needed anymore...
    //enum Direction result = East;
    return this->direction;
}

void Snake::validate_direction()
{
    if (next_direction != Error)
    {
        update_direction(next_direction);
    }
}

/*
This is the main function of the Snake object while it
is also the "engine" behind this entire application "loop".
Each time a new direction is entered by the user, we should 
get here from the main loop. This function will do all the
"housekeeping" to progress the game.
*/
void Snake::update_movement(IN enum Direction direction)
{
    pair<int, int> movement_part;
    switch (direction)
    {
    case West:
        movement_part = make_pair(snake_head.first, snake_head.second - 1);
        break;
    case North:
        movement_part = make_pair(snake_head.first - 1, snake_head.second);
        break;
    case East:
        movement_part = make_pair(snake_head.first, snake_head.second + 1);
        break;
    case South:
        movement_part = make_pair(snake_head.first + 1, snake_head.second);
        break;
    }
    snake_head = movement_part;
    snake_parts.push_back(movement_part);
    food_eaten = snake_head.first == snake_food.first && snake_head.second == snake_food.second;
    if (food_eaten)
    {
        length++;
    }
    else
    {
        pair<int, int> tail = snake_parts.front();
        snake_world_array[tail.first][tail.second]--;
        snake_parts.erase(snake_parts.begin());
    }
    int head_value = ++snake_world_array[snake_head.first][snake_head.second];
    if (head_value > 1)
    {
        is_dead = true;
    }
}

void Snake::set_snake_food(IN pair<int, int> snake_food)
{
    this->snake_food = snake_food;
}

// ==========================================
// GuyA: The new class methods implementation
// ==========================================
SnakeThread::SnakeThread() : Snake()
{
    sem_init(&snake_sema, 0, 1);
    pthread_create(&input_thread, NULL, input_thread_work, this);
}

// GuyA: Copy Ctor of SnakeThread: It is not "clean"
// and defenitly confusing, but in the case of this class
// perhapse having no copy ctor make sense...
// It will, however, MIGHT have other side effects

SnakeThread::~SnakeThread()
{

}

void SnakeThread::update_direction(IN enum Direction direction)
{
    sem_wait(&this->snake_sema);
    Snake::update_direction(direction);
    sem_post(&this->snake_sema);
}

enum Direction SnakeThread::get_direction() 
{
    enum Direction result = East;
    sem_wait(&this->snake_sema);
    result = Snake::get_direction();
    sem_post(&this->snake_sema);
    return result;
}

void SnakeThread::update_movement(IN enum Direction direction)
{
    direction = get_direction();
    Snake::update_movement(direction);
}