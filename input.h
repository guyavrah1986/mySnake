#pragma once

enum Direction {
    Error = -1,
    West = 0,
    North = 1,
    East = 2,
    South = 3
};

void input_init();
void input_enter_off();
void input_enter_on();
enum Direction get_input();
