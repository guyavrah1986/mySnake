#pragma once

#include "snake.h"
#include "macros.h"

class SnakeMap
{
public:
  SnakeMap(IN Snake* snake);
  SnakeMap(IN const SnakeMap& other);
  ~SnakeMap();

  void redraw();
  void update_snake_food(IN bool force_update);
  void update_score();
  // GuyA:
  void DisplayPotentialPathToNewApple();
  void findPathRecuersively(IN Snake& snake, IN SnakeMap& snakMap, INOUT std::vector<std::pair<int, int>>& path);
  void displayMapWithPotentialPath(IN std::vector<std::pair<int, int>>& path) const;

private:
  void update_snake_head();
  void clear_map();

public:
  // Members:
  std::pair<int, int> snake_food;

private:
  char map_array[MAP_HEIGHT][MAP_WIDTH] = {MAP_CHAR};
  Snake* snake;
};

bool is_game_end(IN Snake* snake);
bool isItemInVec(IN const std::pair<int, int>& pairToSearch, IN const std::vector<std::pair<int, int>>& vecToSearch);

