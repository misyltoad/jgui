#ifndef JGUI_COLOUR_H
#define JGUI_COLOUR_H

#include <jgui/jgui_common.h>
#include <stdio.h>
#include <string>

namespace jgui
{
	class jgui_export Colour
	{
	public:
		Colour()
			: r(0.0f)
			, g(0.0f)
			, b(0.0f)
			, a(1.0f)
		{

		}

		Colour(u32 hexcode)
		{
			SetFromHex(hexcode);
		}

		Colour(const char* hexCode)
		{
			SetFromString(hexCode);
		}

		Colour(u8 r, u8 g, u8 b, u8 a = 255)
		{
			SetFromRGB(r, g, b, a);
		}

		void SetFromString(const char* string);

		inline void SetFromHex(u32 hexcode)
		{
			u8 r = (hexcode & 0xFF000000) >> 24;
			u8 g = (hexcode & 0x00FF0000) >> 16;
			u8 b = (hexcode & 0x0000FF00) >> 8;
			u8 a = (hexcode & 0x000000FF) >> 0;

			SetFromRGB(r, g, b, a);
		}

		void SetFromHSL(f32 hue, f32 saturation, f32 lightness, f32 alpha = 1.0f);

		inline void SetFromRGBNormalized(f32 r, f32 g, f32 b, f32 a = 1.0f)
		{
			this->r = r;
			this->g = g;
			this->b = b;
			this->a = a;
		}
		inline void SetFromRGB(u8 r, u8 g, u8 b, u8 a = 255)
		{
			this->r = (float)r / 255.0f;
			this->g = (float)g / 255.0f;
			this->b = (float)b / 255.0f;
			this->a = (float)a / 255.0f;
		}

		union {
			float rgba[4];
			struct {
				float r, g, b, a;
			};
		};
	};
}

#endif