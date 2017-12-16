#ifndef JGUI_SVG_IMAGE_H
#define JGUI_SVG_IMAGE_H

#include <jgui/panel.h>

namespace jgui
{
	class SVGImage : public Panel
	{
	public:
		DECLARE_CLASS(SVGImage, Panel);
		SVGImage();

		DECLARE_DATA_DESC();

		QUICK_MEMBER_STRING(Image);
		QUICK_MEMBER_BOOL(Fixup);

		virtual void Paint();
		virtual void RecomputeScale();

	private:
		void* m_SvgData;

		f32 m_Mins[2];
		f32 m_Maxs[2];
	};
}

#endif