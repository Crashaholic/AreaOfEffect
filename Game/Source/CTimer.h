#pragma once

#include <chrono>

class Timer
{
private:
	std::chrono::time_point<std::chrono::steady_clock> start;

public:

	void StartTimer()
	{
		start = std::chrono::high_resolution_clock::now();
	}

	double Lap()
	{
		auto end = std::chrono::high_resolution_clock::now();
		auto s = std::chrono::time_point_cast<std::chrono::milliseconds>(start).time_since_epoch();
		auto e = std::chrono::time_point_cast<std::chrono::milliseconds>(end).time_since_epoch();
		std::chrono::duration<float> dur = e - s;
		return dur.count();
	}

};