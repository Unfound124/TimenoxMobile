// Timenox Copyright (c) Mutex Studios


#include "TimenoxFunctionLib.h"
#include <regex>

bool UTimenoxFunctionLib::isEmailValid(const FString& email) {
	const std::regex pattern
	("(\\w+)(\\.|_)?(\\w*)@(\\w+)(\\.(\\w+))+");

	return std::regex_match(TCHAR_TO_UTF8(*email), pattern);
}
