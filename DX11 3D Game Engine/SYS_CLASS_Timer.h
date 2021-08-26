#pragma once
#include <chrono>

class EngineTimer
{
public:
	EngineTimer();
	float Mark();
	float Peek() const;
private:
	std::chrono::steady_clock::time_point last;
};