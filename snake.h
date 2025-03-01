#pragma once

#include <semaphore.h>
#include <utility>
#include <vector>
#include "input.h"
#include "macros.h"

/*
This class is a "slimmer" version of the "original" Snake class
but without the relevant stuff for synchronizing and using an
additional thread - they were moved to the derived class of it
so that this class can be utilized for the "green path" calculation.
*/
class Snake
{
public:
  Snake();
  virtual ~Snake();
  Snake(IN const Snake& other);

  virtual void update_movement(IN enum Direction direction);
  virtual void update_direction(IN enum Direction direction);
  virtual enum Direction get_direction();
  void update_next_direction(IN enum Direction direction);
  void set_snake_food(IN std::pair<int, int> snake_food);
  void validate_direction();

  std::vector<std::pair<int, int>> snake_parts;
  std::pair<int, int> snake_head;
  bool food_eaten;
  bool is_dead;
  int length;

private:
  enum Direction direction;
  enum Direction next_direction;
  std::pair<int, int> snake_food;
  int snake_world_array[MAP_HEIGHT][MAP_WIDTH] = {0};
  
  void initialize_snake();
};

/*
This is the "new" version of the "original" Snake class
with its synchronization and threading members.
In order to be able to utilize the different "pieces" of
logic of this class (combined with the SnakeMap class) to
use it in the recursion, several adjusments were made to
some of the functions.
*/
class SnakeThread : public Snake
{
  public:
      SnakeThread();
      virtual ~SnakeThread();

      // Overridden functions:
      // ---------------------
      // Thread safe versions of their parent's implementaiton
      virtual void update_direction(IN enum Direction direction) override;
      virtual enum Direction get_direction() override;
      virtual void update_movement(IN enum Direction direction) override;
      
  private:
      pthread_t input_thread;
      sem_t snake_sema;
};