#include <jgui/label.h>

namespace jgui
{
	const char* defaultFont = "default";

	START_DATA_DESC(Label)
		DATA_DESC_DATA_FUNC(Font, Label::SetFont)
		DATA_DESC_DATA_FUNC(Text, Label::SetText)
		DATA_DESC_DATA_FUNC(Size, Label::SetSize)
		DATA_DESC_DATA_FUNC(Colour, Label::SetTextColour)
		DATA_DESC_DATA_FUNC(Color, Label::SetTextColour)
		DATA_DESC_DATA_FUNC(TextColour, Label::SetTextColour)
		DATA_DESC_DATA_FUNC(TextColor, Label::SetTextColour)
		DATA_DESC_DATA_FUNC(GlowColour, Label::SetGlowColour)
		DATA_DESC_DATA_FUNC(GlowColor, Label::SetGlowColour)
		DATA_DESC_DATA_FUNC(Blur, Label::SetBlur)
		DATA_DESC_DATA_FUNC(Glow, Label::SetGlow)
		DATA_DESC_DATA_FUNC(Align, Label::SetAlignment)
		DATA_DESC_DATA_FUNC(Alignment, Label::SetAlignment)
	END_DATA_DESC(Label)

	Label::Label()
		: Text(nullptr)
		, Font(defaultFont)
		, TextColour(255, 255, 255)
		, Blur(0.0f)
		, Size(18.0f)
		, Glow(false)
		, GlowColour(255, 255, 255)
		, AlignX(TextAlignment::Left)
		, AlignY(TextAlignment::Top)
	{
	}
	Label::~Label()
	{
		if (Text)
			free((void*)Text);
	}
	void Label::Paint()
	{
		Panel::Paint();

		BeginPath();
		auto& bounds = GetBounds();

		if (!Text || !Font)
			return;

		if (Glow)
		{
			f32 renderX, renderY;
			AddText(Font, Text, (f32)bounds.x, (f32)bounds.y, Size, 0.0f, bounds.width, bounds.height, AlignX, AlignY, &renderX, &renderY, false);

			FillColour(GlowColour);
			AddText(Font, Text, renderX, renderY, Size, Blur, bounds.width, bounds.height, TextAlignment::Left, TextAlignment::Top);

			FillColour(TextColour);
			AddText(Font, Text, renderX, renderY, Size, 0, bounds.width, bounds.height, TextAlignment::Left, TextAlignment::Top);
		}
		else
		{
			FillColour(TextColour);
			AddText(Font, Text, (f32)bounds.x, (f32)bounds.y, Size, Blur, bounds.width, bounds.height, AlignX, AlignY);
		}

		FillPath();
	}
	void Label::SetFont(const char* font)
	{
		if (Font == font)
			return;

		if (Font && Font != defaultFont)
		{
			free((void*)Font);
			Font = nullptr;
		}

		if (!font)
			return;

		usize size = strlen(font) + 1;
		char* allocFont = (char*)malloc(size);
		if (allocFont)
		{
			memcpy(allocFont, font, size);
			Font = allocFont;
		}
	}
	void Label::SetText(const char* text)
	{
		if (Text == text)
			return;

		if (Text)
		{
			free((void*)Text);
			Text = nullptr;
		}

		if (!text)
			return;

		usize size = strlen(text) + 1;
		char* allocText = (char*)malloc(size);
		if (allocText)
		{
			memcpy(allocText, text, size);
			Text = allocText;
		}
	}
	void Label::SetBlur(float blur)
	{
		Blur = blur;
		Glow = false;
	}
	void Label::SetGlow(float blur)
	{
		Blur = blur;
		Glow = true;
	}
	void Label::SetSize(float size)
	{
		Size = size;
	}
	void Label::SetGlowColour(jgui::Colour colour)
	{
		GlowColour = colour;
	}
	void Label::SetTextColour(jgui::Colour colour, bool updateGlowColour)
	{
		TextColour = colour;
		if (updateGlowColour)
			GlowColour = colour;
	}

	void Label::SetAlignment(const char* data)
	{
		std::string strData = data;
		auto list = comma_list_to_strings(strData);
		if (list.size() != 2)
			return;

		if (list[0] == "Left")
			AlignX = TextAlignment::Left;
		else if (list[0] == "Center" || list[0] == "Centre")
			AlignX = TextAlignment::Center;
		else if (list[0] == "Right")
			AlignX = TextAlignment::Right;

		if (list[1] == "Top")
			AlignY = TextAlignment::Top;
		else if (list[1] == "Middle")
			AlignY = TextAlignment::Middle;
		else if (list[1] == "Bottom")
			AlignY = TextAlignment::Bottom;
	}

	void Label::SetAlignment(TextAlignmentX x, TextAlignmentY y)
	{
		AlignX = x;
		AlignY = y;
	}
}