#pragma once

// Includes
#include "Log.h"

#include <vector>
#include <string>
#include <memory>
#include "Math/vectors.h"

// Functions
static std::wstring AddStringsReturnWStr(const std::string aString1, const std::string aString2)
{
	std::string newString(aString1 + aString2);
	std::wstring newWString(newString.begin(), newString.end());

	return newWString;
}

// Defines
#define enableImGui
#define enableAssert