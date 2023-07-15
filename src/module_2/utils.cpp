#include "./utils.hpp"

namespace utils
{
	bool hascmd (
		const std::string& mainString,
		const std::string& subString
	) {
		// If the subString is longer than mainString, it cannot be contained 
		if (mainString.length() < subString.length()) {
			return false;
		}

		// Look for the first non-whitespace character
		int index;
		for (index = 0; index < mainString.length(); index++) {
			if (!std::isspace(mainString[index])) {
				break;
			}
		}

		// If the string has nothing but whitespace, no meaningful comparison 
		// can be made
		if (index >= mainString.length()) {
			return false;
		}

		// Compare the strings
		return mainString.substr(index, subString.length()) == subString;
	}
}
