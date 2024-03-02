#pragma once
#include <iostream>
#include <windows.h>

enum LogType
{
	eStandard,
	eError,
	eSuccess
};

namespace Log
{
	static void Print(std::string aString, LogType aLogType = LogType::eStandard)
	{
#define RED 12
#define GREEN 10
#define YELLOW 14
#define WHITE 15

		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

		switch (aLogType)
		{
			case eStandard:
				std::cout << aString << std::endl;
				break;

			case eError:
				SetConsoleTextAttribute(hConsole, RED);
				std::cout << aString << std::endl;
				break;

			case eSuccess:
				SetConsoleTextAttribute(hConsole, GREEN);
				std::cout << aString << std::endl;
				break;

			default:
				break;

		}

		SetConsoleTextAttribute(hConsole, WHITE);
	}
}

#define LOG(string) Log::Print(string, LogType::eStandard)
#define LOG_SUCCESS(string) Log::Print(string, LogType::eSuccess)
#define LOG_ERROR(string) Log::Print(string, LogType::eError)

