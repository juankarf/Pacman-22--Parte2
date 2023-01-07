#pragma once
#include "Map.h"
#include "Enemy.h"
#include "TimeManager.h"
#include <vector>

class Variables
{
public:
	enum USER_INPUTS { NONE, UP, DOWN, RIGHT, LEFT, QUIT };
	Map pacman_map = Map();
	std::vector<Enemy> enemigos = std::vector<Enemy>();
	char player_char = 'O';
	int player_x = 1;
	int player_y = 1;
	int player_points = 0;
	int vidas = 3;
	USER_INPUTS input = USER_INPUTS::NONE;
	int points_powerups = 10;
	bool run = true;
	bool win = false;
	bool lose = false;
};

