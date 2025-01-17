#include "Map.h"


Map::Map()
{
	std::string filePath;
	std::cout << "Write file path:" << std::endl;
	std::cin >> filePath;
	std::ifstream file(filePath);
	std::string line;
	std::vector<std::string> lines;
	while (std::getline(file, line))
	{
		lines.push_back(line);
	}
	file.close();
	Width = 0;
	Height = 0;
	Size = 0;
	std::vector<std::string> Map_Load;
	for (size_t i = 0; i < lines.size(); i++)
	{
		if (i == 0)
		{
			std::string WidthTemp;
			std::string HeightTemp;
			int commaCount = 0;
			for (size_t j = 0; j < lines[i].size(); j++)
			{
				if (lines[i][j] == ',')
				{
					commaCount++;
				}
				else
				{
					if (commaCount == 0)
					{
						WidthTemp += lines[i][j];
					}
					else
					{
						HeightTemp += lines[i][j];
					}
				}
			}
			Width = std::stoi(WidthTemp);
			Height = std::stoi(HeightTemp);
			Size = Width * Height;
		}
		else
		{
			Map_Load.push_back(lines[i]);
		}
	}
	std::cout << "Map: " << Size << std::endl;
	std::cout << "  Width: " << Width << std::endl;
	std::cout << "  Height: " << Height << std::endl;
	for (size_t i = 0; i < Map_Load.size(); i++)
	{
		std::cout << Map_Load[i] << std::endl;
	}
	storedmap = new MAP_TILES[Size];
	points = 0;
	int x = 0;
	int y = 0;
	char tile = ' ';
	for (size_t i = 0; i < Size; i++)
	{
		x = i % Width;
		y = i / Width;
		tile = Map_Load[y][x];
		switch (tile)
		{
		case MAP_TILES::MAP_POINT:
			points++;
			storedmap[i] = (MAP_TILES)tile;
			break;
		case 'P':
			spawn_player.X = x;
			spawn_player.Y = y;
			storedmap[i] = MAP_TILES::MAP_EMPTY;
			break;
		case 'E':
			spawn_enemy.X = x;
			spawn_enemy.Y = y;
			storedmap[i] = MAP_TILES::MAP_EMPTY;
			break;
		default:
			storedmap[i] = (MAP_TILES)tile;
			break;
		}
	}
	foregrounds[MAP_TILES::MAP_EMPTY] = ConsoleUtils::CONSOLE_COLOR::BLACK;
	foregrounds[MAP_TILES::MAP_WALL] = ConsoleUtils::CONSOLE_COLOR::DARK_BLUE;
	foregrounds[MAP_TILES::MAP_POINT] = ConsoleUtils::CONSOLE_COLOR::DARK_YELLOW;
	foregrounds[MAP_TILES::MAP_POWERUP] = ConsoleUtils::CONSOLE_COLOR::DARK_YELLOW;
	backgrounds[MAP_TILES::MAP_EMPTY] = ConsoleUtils::CONSOLE_COLOR::BLACK;
	backgrounds[MAP_TILES::MAP_WALL] = ConsoleUtils::CONSOLE_COLOR::DARK_BLUE;
	backgrounds[MAP_TILES::MAP_POINT] = ConsoleUtils::CONSOLE_COLOR::BLACK;
	backgrounds[MAP_TILES::MAP_POWERUP] = ConsoleUtils::CONSOLE_COLOR::DARK_MAGENTA;
}

Map::~Map()
{
	delete[] storedmap;
}

void Map::Draw()
{
	for (size_t i = 0; i < Size; i++)
	{
		if (i % Width == 0 && i > 0)
		{
			ConsoleUtils::Console_SetColor();
			std::cout << ' ' << std::endl;
		}
		MAP_TILES tile = storedmap[i];
		ConsoleUtils::Console_SetColor(foregrounds[tile], backgrounds[tile]);
		std::cout << (char)tile;
	}
	ConsoleUtils::Console_SetColor();
}

int inline Map::GetPos(int _x, int _y)
{
	int pos = _y * Width + _x;
	if (Size <= pos)
		return Size - 1;
	return pos;
}

Map::MAP_TILES Map::GetTile(int _x, int _y)
{
	int pos = GetPos(_x, _y);
	return storedmap[pos];
}

void Map::SetTile(int _x, int _y, MAP_TILES _tile)
{
	int pos = GetPos(_x, _y);
	storedmap[pos] = _tile;
}
