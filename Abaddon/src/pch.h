#pragma once

#include "Log.h"

#include <vector>
#include <string>

static std::wstring AddStringsReturnWStr(const std::string aString1, const std::string aString2)
{
	std::string newString(aString1 + aString2);
	std::wstring newWString(newString.begin(), newString.end());

	return newWString;
}
