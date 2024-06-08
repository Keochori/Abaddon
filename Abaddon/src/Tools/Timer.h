#pragma once
#include <chrono>

class Timer
{
public:
	static Timer& GetInstance();

	Timer();
	Timer(const Timer& aTimer) = delete;
	Timer& operator=(const Timer& aTimer) = delete;
	void Update();
	float GetDeltaTime() const;
	double GetTotalTime() const;

private:
	static Timer* myInstance;

	std::chrono::high_resolution_clock::time_point myStartTime;
	std::chrono::high_resolution_clock::time_point myLastFrameTime;
	double myDeltaTime;
	double myTotalTime;
};
