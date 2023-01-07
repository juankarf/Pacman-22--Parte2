#include "Enemy.h"
#include "TimeManager.h"
#include <vector>
#include "Variables.h"

/// <summary>
/// Sets the needed variables
/// </summary>
void Setup();
/// <summary>
/// Handles the inputs
/// </summary>
void Input();
/// <summary>
/// Handles the logic of the game
/// </summary>
void Logic();
/// <summary>
/// Draws the screen
/// </summary>
void Draw();

Variables info;

int main()
{
    Setup();
    while (info.run)
    {
        Input();
        Logic();
        Draw();
    }
}

void Setup()
{
    std::cout.sync_with_stdio(false);

    srand(time(NULL));

    int enemy_count = 0;

    std::cout << "How many enemies do you want?" << std::endl;
    std::cin >> enemy_count;

    for (size_t i = 0; i < enemy_count; i++)
    {
        info.enemigos.push_back(Enemy(info.pacman_map.spawn_enemy));
    }

    info.player_x = info.pacman_map.spawn_player.X;
    info.player_y = info.pacman_map.spawn_player.Y;
}

void Input()
{
    info.input = info.USER_INPUTS::NONE;
    if (ConsoleUtils::KeyPressed(VK_UP) || ConsoleUtils::KeyPressed('W'))
    {
        info.input = info.USER_INPUTS::UP;
    }
    if (ConsoleUtils::KeyPressed(VK_DOWN) || ConsoleUtils::KeyPressed('S'))
    {
        info.input = info.USER_INPUTS::DOWN;
    }
    if (ConsoleUtils::KeyPressed(VK_RIGHT) || ConsoleUtils::KeyPressed('D'))
    {
        info.input = info.USER_INPUTS::RIGHT;
    }
    if (ConsoleUtils::KeyPressed(VK_LEFT) || ConsoleUtils::KeyPressed('A'))
    {
        info.input = info.USER_INPUTS::LEFT;
    }
    if (ConsoleUtils::KeyPressed(VK_ESCAPE) || ConsoleUtils::KeyPressed('Q'))
    {
        info.input = info.USER_INPUTS::QUIT;
    }
}

void Logic()
{
    if (info.lose) {
        switch (info.input)
        {
        case info.QUIT:
            info.run = false;
            break;
        }
    }
    else {
        if (info.win)
        {
            switch (info.input)
            {
            case info.QUIT:
                info.run = false;
                break;
            }
        }
        else
        {
            COORD playerPos;
            playerPos.X = info.player_x;
            playerPos.Y = info.player_y;
            bool playerDie = false;
            for (size_t i = 0; i < info.enemigos.size(); i++)
            {
                if (info.enemigos[i].Logic(&info.pacman_map, playerPos)) {
                    playerDie = true;
                }
            }
            if (playerDie) {
                info.player_x = info.pacman_map.spawn_player.X;
                info.player_y = info.pacman_map.spawn_player.Y;
                info.vidas--;
            }
            int player_y_new = info.player_y;
            int player_x_new = info.player_x;
            switch (info.input)
            {
            case info.UP:
                player_y_new--;
                break;
            case info.DOWN:
                player_y_new++;
                break;
            case info.RIGHT:
                player_x_new++;
                break;
            case info.LEFT:
                player_x_new--;
                break;
            case info.QUIT:
                info.run = false;
                break;
            }
            if (player_x_new < 0)
            {
                player_x_new = info.pacman_map.Width - 1;
            }
            player_x_new %= info.pacman_map.Width;
            if (player_y_new < 0)
            {
                player_y_new = info.pacman_map.Height - 1;
            }
            player_y_new %= info.pacman_map.Height;

            switch (info.pacman_map.GetTile(player_x_new, player_y_new))
            {
            case Map::MAP_TILES::MAP_WALL:
                player_y_new = info.player_y;
                player_x_new = info.player_x;
                break;
            case Map::MAP_TILES::MAP_POINT:
                info.pacman_map.points--;
                info.player_points++;
                info.pacman_map.SetTile(player_x_new, player_y_new, Map::MAP_TILES::MAP_EMPTY);
                break;
            case Map::MAP_TILES::MAP_POWERUP:
                info.player_points+= info.points_powerups;
                info.pacman_map.SetTile(player_x_new, player_y_new, Map::MAP_TILES::MAP_EMPTY);
                break;
            }

            info.player_y = player_y_new;
            info.player_x = player_x_new;
            if (info.pacman_map.points <= 0)
            {
                info.win = true;
            }
        }
        if (info.vidas <= 0)
        {
            info.lose = true;
        }
    }
    
}

void Draw()
{
    ConsoleUtils::Console_SetPos(0,0);
    info.pacman_map.Draw();
    for (size_t i = 0; i < info.enemigos.size(); i++)
    {
        info.enemigos[i].Draw();
    }
    ConsoleUtils::Console_SetPos(info.player_x, info.player_y);
    ConsoleUtils::Console_SetColor(ConsoleUtils::CONSOLE_COLOR::DARK_YELLOW);
    std::cout << info.player_char;
    ConsoleUtils::Console_ClearCharacter({ 0,(short)info.pacman_map.Height });
    ConsoleUtils::Console_SetColor(ConsoleUtils::CONSOLE_COLOR::CYAN);
    std::cout << "Puntuacion actual: " << info.player_points << " Puntuacion pendiente: " << info.pacman_map.points << std::endl;
    
    std::cout << "Fotogramas: " << TimeManager::getInstance().frameCount << std::endl;
    std::cout << "DeltaTime: " << TimeManager::getInstance().deltaTime << std::endl;
    std::cout << "Time: " << TimeManager::getInstance().time << std::endl;
    std::cout <<"Vidas: " << info.vidas << std::endl;
    if (info.win)
    {
        ConsoleUtils::Console_SetColor(ConsoleUtils::CONSOLE_COLOR::GREEN);
        std::cout << "Has ganado!" << std::endl;
    }
    if (info.lose)
    {
        ConsoleUtils::Console_SetColor(ConsoleUtils::CONSOLE_COLOR::WHITE);
        std::cout << "Has perdido!" << std::endl;
    }

    TimeManager::getInstance().nextFrame();
}