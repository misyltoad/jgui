#include <jgui/label.h>
#include <jgui_private/string_helpers.h>

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
		: m_Text("")
		, m_Font(defaultFont)
		, m_TextColour(255, 255, 255)
		, m_Blur(0.0f)
		, m_Size(18.0f)
		, m_Glow(false)
		, m_GlowColour(255, 255, 255)
		, m_AlignX(TextAlignment::Left)
		, m_AlignY(TextAlignment::Top)
	{
	}
	Label::~Label()
	{
	}
	void Label::Paint()
	{
		Panel::Paint();

		BeginPath();
		auto& bounds = GetBounds();

		if (m_Text.empty() || m_Font.empty())
			return;

		if (m_Glow)
		{
			f32 renderX, renderY;
			AddText(m_Font.c_str(), m_Text.c_str(), (f32)bounds.x, (f32)bounds.y, m_Size, 0.0f, (f32)bounds.width, (f32)bounds.height, m_AlignX, m_AlignY, &renderX, &renderY, false);

			FillColour(m_GlowColour);
			AddText(m_Font.c_str(), m_Text.c_str(), renderX, renderY, m_Size, m_Blur, (f32)bounds.width, (f32)bounds.height, TextAlignment::Left, TextAlignment::Top);

			FillColour(m_TextColour);
			AddText(m_Font.c_str(), m_Text.c_str(), renderX, renderY, m_Size, 0, (f32)bounds.width, (f32)bounds.height, TextAlignment::Left, TextAlignment::Top);
		}
		else
		{
			FillColour(m_TextColour);
			AddText(m_Font.c_str(), m_Text.c_str(), (f32)bounds.x, (f32)bounds.y, m_Size, m_Blur, (f32)bounds.width, (f32)bounds.height, m_AlignX, m_AlignY);
		}

		FillPath();
	}

	void Label::SetAlignment(const char* data)
	{
		SetAlignment(std::string(data));
	}

	void Label::SetAlignment(const std::string& value)
	{
		auto list = comma_list_to_strings(value);
		if (list.size() != 2)
			return;

		if (list[0] == "Left")
			m_AlignX = TextAlignment::Left;
		else if (list[0] == "Center" || list[0] == "Centre")
			m_AlignX = TextAlignment::Center;
		else if (list[0] == "Right")
			m_AlignX = TextAlignment::Right;

		if (list[1] == "Top")
			m_AlignY = TextAlignment::Top;
		else if (list[1] == "Middle")
			m_AlignY = TextAlignment::Middle;
		else if (list[1] == "Bottom")
			m_AlignY = TextAlignment::Bottom;
	}

	void Label::SetAlignment(TextAlignmentX x, TextAlignmentY y)
	{
		m_AlignX = x;
		m_AlignY = y;
	}

	QUICK_MEMBER_IMPLEMENT_STRING_SIMPLE(Label, Font)
	QUICK_MEMBER_IMPLEMENT_STRING_SIMPLE(Label, Text)
	QUICK_MEMBER_IMPLEMENT_FLOAT(Label, Blur)
	{
		m_Blur = value;
		m_Glow = false;
	}

	QUICK_MEMBER_IMPLEMENT_BOOL_SIMPLE(Label, Glow)
	QUICK_MEMBER_IMPLEMENT_FLOAT_SIMPLE(Label, Size)
	QUICK_MEMBER_IMPLEMENT_COLOUR_SIMPLE(Label, GlowColour)
	QUICK_MEMBER_IMPLEMENT_COLOUR_SIMPLE(Label, TextColour)
}