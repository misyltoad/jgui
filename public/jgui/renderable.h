#ifndef JGUI_RENDERABLE_H
#define JGUI_RENDERABLE_H

#include <jgui/jgui_common.h>
#include <jgui/colour.h>

namespace jgui
{
	namespace TextAlignment
	{
		enum EnumX
		{
			Left,
			Center,
			Right
		};

		enum EnumY
		{
			Top,
			Middle,
			Bottom
		};
	}
	using TextAlignmentX = TextAlignment::EnumX;
	using TextAlignmentY = TextAlignment::EnumY;

	class jgui_export Renderable
	{
	public:
		float GetScaleX();
		float GetScaleY();

		void BeginPath();

		void AddRect(f32 x, f32 y, f32 width, f32 height, f32 roundness = 0.0f);

		void AddText(const char* fontName, const char* text, f32 x, f32 y, f32 size, f32 blur = 0.0f, f32 width = -1.0f, f32 height = -1.0f, TextAlignmentX alignX = TextAlignment::Left, TextAlignmentY alignY = TextAlignment::Top, f32* outX = nullptr, f32* outY = nullptr, bool render = true);

		void FillColour(Colour colour);
		void StrokeColour(Colour colour);

		void StrokeThickness(f32 thickness);

		void MoveTo(f32 x, f32 y);
		void BezierTo(f32 c1x, f32 c1y, f32 c2x, f32 c2y, f32 x, f32 y);
		void LineTo(f32 x, f32 y);

		void FillPath();
		void StrokePath();

	protected:
		virtual void* GetRenderContext() = 0;
		f32 scaleX, scaleY;
		f32 xOffset, yOffset;
	};
}

#endif