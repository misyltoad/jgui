#ifndef JGUI_BOUNDS_H
#define JGUI_BOUNDS_H

#include <jgui/jgui_common.h>

namespace jgui
{
	struct Bounds
	{
		static constexpr f32 InvalidPosition = -1;

		f32 x = InvalidPosition, y = InvalidPosition;
		f32 width = 0, height = 0;
	};
}

#endif