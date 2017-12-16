#include <jgui/svg_image.h>
#include <jgui/window.h>

#define NANOSVG_ALL_COLOR_KEYWORDS
#define NANOSVG_IMPLEMENTATION
#include "nanosvg.h"

namespace jgui
{
	SVGImage::SVGImage()
		: m_SvgData(nullptr)
	{
		SetMaintainAspectRatio(true);
		SetFixup(true);
	}

	START_DATA_DESC(SVGImage)
		DATA_DESC_DATA_FUNC(Image, SVGImage::SetImage)
		DATA_DESC_DATA_FUNC(Fixup, SVGImage::SetFixup)
	END_DATA_DESC(SVGImage)

	QUICK_MEMBER_IMPLEMENT_BOOL_SIMPLE(SVGImage, Fixup);

	QUICK_MEMBER_IMPLEMENT_STRING(SVGImage, Image)
	{
		if (m_Image == value)
			return;

		m_SvgData = nullptr;
		m_Image = value;

		m_SvgData = (void*)nsvgParseFromFile(m_Image.c_str(), "px", 96.0f);
		RecomputeScale();
	}

	inline float MinIfGreaterThanZero(f32 a, f32 b)
	{
		if (a <= 0)
			return b;

		if (b <= 0)
			return a;

		return MIN(a, b);
	}

	inline Colour NSVGColourToJGUIColour(u32 colour)
	{
		u8 a = (colour & 0xFF000000) >> 24;
		u8 b = (colour & 0x00FF0000) >> 16;
		u8 g = (colour & 0x0000FF00) >> 8;
		u8 r = (colour & 0x000000FF) >> 0;

		return Colour(r, g, b, a);
	}

	void SVGImage::RecomputeScale()
	{
		BaseClass::RecomputeScale();

		if (!m_SvgData)
			return;

		auto* image = reinterpret_cast<NSVGimage*>(m_SvgData);

		NSVGshape* shape = nullptr;
		NSVGpath* path = nullptr;
		int i;

		if (m_Fixup)
		{
			for (shape = image->shapes; shape != NULL; shape = shape->next)
			{
				if (!(shape->flags & NSVG_FLAGS_VISIBLE))
					continue;

				for (path = shape->paths; path != NULL; path = path->next)
				{
					m_Mins[0] = MinIfGreaterThanZero(path->pts[0], m_Mins[0]);
					m_Mins[1] = MinIfGreaterThanZero(path->pts[1], m_Mins[1]);

					m_Maxs[0] = MAX(path->pts[0], m_Maxs[0]);
					m_Maxs[1] = MAX(path->pts[1], m_Maxs[1]);

					for (i = 0; i < path->npts - 1; i += 3)
					{
						float* p = &path->pts[i * 2];

						m_Mins[0] = MinIfGreaterThanZero(p[2], m_Mins[0]);
						m_Mins[1] = MinIfGreaterThanZero(p[3], m_Mins[1]);

						m_Mins[0] = MinIfGreaterThanZero(p[4], m_Mins[0]);
						m_Mins[1] = MinIfGreaterThanZero(p[5], m_Mins[1]);

						m_Mins[0] = MinIfGreaterThanZero(p[6], m_Mins[0]);
						m_Mins[1] = MinIfGreaterThanZero(p[7], m_Mins[1]);

						// --

						m_Maxs[0] = MAX(p[2], m_Maxs[0]);
						m_Maxs[1] = MAX(p[3], m_Maxs[1]);

						m_Maxs[0] = MAX(p[4], m_Maxs[0]);
						m_Maxs[1] = MAX(p[5], m_Maxs[1]);

						m_Maxs[0] = MAX(p[6], m_Maxs[0]);
						m_Maxs[1] = MAX(p[7], m_Maxs[1]);

					}
				}
			}

			auto& ourBounds = GetBounds();
			f32 imageDimensions[2] = { m_Maxs[0] - m_Mins[0], m_Maxs[1] - m_Mins[1] };
			if (GetMaintainAspectRatio())
			{
				f32 maxDimension = MAX(imageDimensions[0], imageDimensions[1]);

				f32 minBound = MIN(ourBounds.width, ourBounds.height);
				f32 contantScalar = minBound / maxDimension;

				scaleX *= contantScalar;
				scaleY *= contantScalar;
			}
			else
			{
				scaleX *= ourBounds.width / imageDimensions[0];
				scaleY *= ourBounds.height / imageDimensions[1];
			}
		}
	}

	void SVGImage::Paint()
	{
		BaseClass::Paint();

		if (!m_SvgData)
			return;

		auto* image = reinterpret_cast<NSVGimage*>(m_SvgData);

		NSVGshape* shape = nullptr;
		NSVGpath* path = nullptr;
		int i;

		xOffset -= m_Mins[0]; //* scaleX;
		yOffset -= m_Mins[1]; //* scaleY;

		for (shape = image->shapes; shape != NULL; shape = shape->next) 
		{
			if (!(shape->flags & NSVG_FLAGS_VISIBLE))
				continue;

			FillColour(NSVGColourToJGUIColour(shape->fill.color));
			StrokeColour(NSVGColourToJGUIColour(shape->stroke.color));
			StrokeThickness(shape->strokeWidth);

			for (path = shape->paths; path != NULL; path = path->next) 
			{
				BeginPath();
				MoveTo(path->pts[0], path->pts[1]);
				for (i = 0; i < path->npts - 1; i += 3) 
				{
					float* p = &path->pts[i * 2];
					BezierTo(p[2], p[3], p[4], p[5], p[6], p[7]);
				}
				if (path->closed)
					LineTo(path->pts[0], path->pts[1]);

				if (shape->fill.type)
					FillPath();

				if (shape->stroke.type)
					StrokePath();
			}
		}
	}
}