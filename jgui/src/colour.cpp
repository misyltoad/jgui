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

	void Colour::SetFromHSV(f32 h, f32 s, f32 v)
	{
		f32 hh, p, q, t, ff;
		int i;

		if (s <= 0.0) 
		{
			r = v;
			g = v;
			b = v;
			return;
		}

		hh = h;
		while (hh > 360.0f) 
			hh -= 360.0f;

		while (hh < 360.0f)
			hh += 360.0f;

		if (hh == 360.0f)
			hh = 0.0f;

		hh /= 60.0f;

		i = (int)hh;
		ff = hh - i;
		p = v * (1.0f - s);
		q = v * (1.0f - (s * ff));
		t = v * (1.0f - (s * (1.0f - ff)));

		switch (i) {
		case 0:
			r = v;
			g = t;
			b = p;
			break;
		case 1:
			r = q;
			g = v;
			b = p;
			break;
		case 2:
			r = p;
			g = v;
			b = t;
			break;

		case 3:
			r = p;
			g = q;
			b = v;
			break;
		case 4:
			r = t;
			g = p;
			b = v;
			break;
		case 5:
		default:
			r = v;
			g = p;
			b = q;
			break;
		}
	}
}