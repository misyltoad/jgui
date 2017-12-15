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
		
		QUICK_MEMBER_STRING(Font);
		QUICK_MEMBER_STRING(Text);
		QUICK_MEMBER_FLOAT(Blur);
		QUICK_MEMBER_BOOL(Glow);
		QUICK_MEMBER_FLOAT(Size);
		QUICK_MEMBER_COLOUR(GlowColour);
		QUICK_MEMBER_COLOUR(TextColour);

		void SetAlignment(const char* value);
		void SetAlignment(const std::string& value);
		void SetAlignment(TextAlignmentX x, TextAlignmentY y);

	protected:
		TextAlignmentX m_AlignX;
		TextAlignmentY m_AlignY;
	};
}

#endif