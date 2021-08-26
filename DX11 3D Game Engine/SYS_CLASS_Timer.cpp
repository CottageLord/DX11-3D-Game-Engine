#include "SYS_CLASS_Timer.h"
using namespace std::chrono;

EngineTimer::EngineTimer()
{
	last = steady_clock::now();
}

float EngineTimer::Mark()
{
	const auto old = last;
	last = steady_clock::now();
	const duration<float> frameTime = last - old;
	return frameTime.count();
}

float EngineTimer::Peek() const
{
	return duration<float>(steady_clock::now() - last).count();
}