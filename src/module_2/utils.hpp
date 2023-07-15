#pragma once

#include <string>
#include <cctype>

#define ever (;;)

namespace utils
{
	bool hascmd(
		const std::string &mainString,
		const std::string &subString);
}