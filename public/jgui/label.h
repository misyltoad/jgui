#ifndef JGUI_LABEL_H
#define JGUI_LABEL_H

#include <jgui/panel.h>

namespace jgui
{
	class jgui_export Label : public Panel
	{
	public:
		DECLARE_CLASS(Label, Panel);
		DECLARE_DATA_DESC();

		Label();
		virtual ~Label();

		virtual void Paint();
		void SetFont(const char* font);
		void SetText(const char* text);
		void SetBlur(float blur);
		DESC_FUNC_FLOAT_WRAPPER(SetBlur);
		void SetGlow(float blur);
		DESC_FUNC_FLOAT_WRAPPER(SetGlow);
		void SetSize(float size);
		DESC_FUNC_FLOAT_WRAPPER(SetSize);

		void SetTextColour(Colour colour, bool updateGlowColor = false);
		DESC_FUNC_COLOUR_WRAPPER(SetTextColour);
		void SetGlowColour(Colour colour);
		DESC_FUNC_COLOUR_WRAPPER(SetGlowColour);

		void SetAlignment(const char* data);
		void SetAlignment(TextAlignmentX x, TextAlignmentY y);

	protected:
		TextAlignmentX AlignX;
		TextAlignmentY AlignY;

		const char* Font;
		const char* Text;
		float Blur;
		float Size;
		bool Glow;
		Colour GlowColour;
		Colour TextColour;
	};
}

#endif