#pragma once

__declspec(align(16))
struct TestCBuffer 
{
	TestCBuffer(unsigned int aTime) 
	{
		myTime = aTime;
	}

	unsigned int myTime;
};