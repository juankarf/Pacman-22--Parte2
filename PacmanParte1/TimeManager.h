#pragma once
#include <chrono>

class TimeManager
{
	std::chrono::high_resolution_clock::time_point start =
		std::chrono::high_resolution_clock::now();
public:
	static TimeManager& getInstance();

	unsigned int frameCount;
	float time;
	float deltaTime;

	void nextFrame();
};

