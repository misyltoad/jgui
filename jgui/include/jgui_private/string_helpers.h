#ifndef JGUI_STRING_HELPERS_H
#define JGUI_STRING_HELPERS_H

#include <jgui/jgui_common.h>

#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include <unordered_map>
#include <algorithm>

namespace jgui
{
	inline std::vector<std::string> split_string(const std::string& str,
		const std::string& delimiter)
	{
		std::vector<std::string> strings;

		std::string::size_type pos = 0;
		std::string::size_type prev = 0;
		while ((pos = str.find(delimiter, prev)) != std::string::npos)
		{
			strings.push_back(str.substr(prev, pos - prev));
			prev = pos + 1;
		}

		// To get the last substring (or only, if delimiter is not found)
		strings.push_back(str.substr(prev));

		return strings;
	}

	inline bool contains(const std::string& string, const std::string& string2)
	{
		return string.find(string2) != std::string::npos;
	}

	inline std::string remove(const std::string& string, char character)
	{
		std::string cpy = string;
		cpy.erase(std::remove(cpy.begin(), cpy.end(), character), cpy.end());
		return cpy;
	}

	inline std::string trim(const std::string& str,
		const std::string& whitespace = " \t")
	{
		const auto strBegin = str.find_first_not_of(whitespace);
		if (strBegin == std::string::npos)
			return ""; // no content

		const auto strEnd = str.find_last_not_of(whitespace);
		const auto strRange = strEnd - strBegin + 1;

		return str.substr(strBegin, strRange);
	}

	inline std::vector<std::string> comma_list_to_strings(std::string strData)
	{
		strData = trim(strData);
		auto vecData = split_string(strData, ",");
		std::vector<std::string> retVal;
		for (std::string value : vecData)
			retVal.push_back((trim(value).c_str()));

		return retVal;
	}

	inline std::vector<int> comma_list_to_numbers(std::string strData)
	{
		strData = trim(strData);
		auto vecData = split_string(strData, ",");
		std::vector<int> retVal;
		for (std::string value : vecData)
			retVal.push_back(atoi(trim(value).c_str()));

		return retVal;
	}
}

#endif