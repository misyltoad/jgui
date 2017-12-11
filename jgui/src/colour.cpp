#include <jgui/colour.h>
#include <jgui_private/string_helpers.h>

namespace jgui
{
	void Colour::SetFromString(const char* str)
	{
		u32 value = 0;
		std::string strStr = std::string(str);
		if (strStr.length() > 0 && strStr[0] == '#')
		{
			strStr = strStr.substr(1);

			sscanf_s(strStr.c_str(), "%x", &value);
			switch (strStr.length())
			{
			case 6:
			{
				value = value << 8;
				value += 0xFF;
				SetFromHex(value);
			} break;

			case 8:
			{
				SetFromHex(value);
			} break;
			}
			return;
		}

		auto list = comma_list_to_numbers(strStr);
		if (list.size() == 3)
			SetFromRGB(list[0], list[1], list[2], 255);
		else if (list.size() == 4)
			SetFromRGB(list[0], list[1], list[2], list[3]);
	}
}