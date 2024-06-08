#include "pch.h"
#include "Timer.h"

Timer* Timer::myInstance = nullptr;

Timer& Timer::GetInstance()
{
	if (!myInstance)
	{
		myInstance = new Timer();
	}

	return *myInstance;
}

Timer::Timer()
{
	myStartTime = std::chrono::high_resolution_clock::now();
	myLastFrameTime = myStartTime;
	myDeltaTime = 0;
	myTotalTime = 0;
}

void Timer::Update()
{
	myTotalTime = std::chrono::duration_cast<std::chrono::duration<double>>(std::chrono::high_resolution_clock::now() - myStartTime).count();
	myDeltaTime = std::chrono::duration_cast<std::chrono::duration<double>>(std::chrono::high_resolution_clock::now() - myLastFrameTime).count();
	myLastFrameTime = std::chrono::high_resolution_clock::now();
}

float Timer::GetDeltaTime() const
{
	return myDeltaTime;
}

double Timer::GetTotalTime() const
{
	return myTotalTime;
}
