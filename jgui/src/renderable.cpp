#include <jgui/renderable.h>
#include <nanovg.h>

namespace jgui
{
	float Renderable::GetScaleX()
	{
		return scaleX;
	}
	float Renderable::GetScaleY()
	{
		return scaleY;
	}

#define vg (NVGcontext*)GetRenderContext()
#define scaleX(x) x = x * scaleX
#define scaleY(y) y = y * scaleY

	void Renderable::BeginPath()
	{
		nvgBeginPath(vg);
	}
	void Renderable::AddRect(f32 x, f32 y, f32 width, f32 height, f32 roundness)
	{
		scaleX(x);
		scaleX(width);

		scaleY(y);
		scaleY(height);

		scaleX(roundness);

		x += xOffset * scaleX;
		y += yOffset * scaleY;

		nvgRoundedRect(vg, x, y, width, height, roundness);
	}
	void Renderable::AddText(const char* fontName, const char* text, f32 x, f32 y, f32 size, f32 blur, f32 width, f32 height, TextAlignmentX alignX, TextAlignmentY alignY, f32* outX, f32* outY, bool render)
	{
		scaleX(x);
		scaleX(width);
		scaleY(y);
		scaleY(size);
		scaleY(blur);
		scaleY(height);

		x += xOffset * scaleX;
		y += yOffset * scaleY;

		nvgFontFace(vg, fontName);
		nvgFontSize(vg, size);
		nvgFontBlur(vg, blur);
		nvgTextAlign(vg, NVG_ALIGN_TOP | NVG_ALIGN_LEFT);

		float bounds[4];
		nvgTextBounds(vg, x, y, text, nullptr, bounds);

		usize newlineCount = 1;
		usize textLen = strlen(text);
		for (usize i = 0; i < textLen; i++)
		{
			if (text[i] == '\n')
				newlineCount++;
		}

		float calcWidth = bounds[2] - bounds[0];
		float calcHeight = (bounds[3] - bounds[1]) * newlineCount;

		if (alignY == TextAlignment::Middle)
			y = y + ((height / 2.0f) - (calcHeight / 2.0f));
		else if (alignY == TextAlignment::Bottom)
			y = y + (height - calcHeight);

		if (alignX == TextAlignment::Center)
			x = x + ((width / 2.0f) - (calcWidth / 2.0f));
		else if (alignX == TextAlignment::Right)
			x = x + (width - calcWidth);

		if (outX)
			*outX = x / scaleX;

		if (outY)
			*outY = y / scaleY;

		if (!render)
			return;

		if (width)
			nvgTextBox(vg, x, y, width, text, nullptr);
		else
			nvgText(vg, x, y, text, nullptr);
	}
	void Renderable::FillColour(Colour colour)
	{
		nvgFillColor(vg, *((NVGcolor*)(&colour)));
	}
	void Renderable::FillPath()
	{
		nvgFill(vg);
	}
	void Renderable::StrokeColour(Colour colour)
	{
		nvgStrokeColor(vg, *((NVGcolor*)(&colour)));
	}
	void Renderable::StrokeThickness(f32 thickness)
	{
		scaleX(thickness);
		nvgStrokeWidth(vg, thickness);
	}
	void Renderable::StrokePath()
	{
		nvgStroke(vg);
	}
}