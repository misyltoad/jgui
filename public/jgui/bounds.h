#ifndef JGUI_BOUNDS_H
#define JGUI_BOUNDS_H

#include <jgui/jgui_common.h>

namespace jgui
{
	struct Bounds
	{
		static constexpr u32 InvalidPosition = (u32)(-1);

		u32 x = InvalidPosition, y = InvalidPosition;
		u32 width = 0, height = 0;
	};
}

#endif