#ifndef JGUI_COMMON_H
#define JGUI_COMMON_H

#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include <unordered_map>

#if _WIN32
#ifdef JGUI_DLL
#define jgui_export __declspec( dllexport )
#else
#define jgui_export __declspec( dllimport )
#endif
#else

#ifdef JGUI_DLL
#define jgui_export __attribute__((visibility("default")))
#else
#define jgui_export
#endif

#endif

#include <algorithm>

namespace jgui
{
	using i8 = signed char;
	using i16 = signed short;
	using i32 = signed int;
	using i64 = signed long long;

	using u8 = unsigned char;
	using u16 = unsigned short;
	using u32 = unsigned int;
	using u64 = unsigned long long;

	using usize = size_t;

	using f32 = float;
	using f64 = double;


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
