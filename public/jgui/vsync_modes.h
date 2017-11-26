#ifndef JGUI_VSYNC_MODES_H
#define JGUI_VSYNC_MODES_H

namespace jgui
{
	namespace VSyncModes
	{
		enum Enum
		{
			Disabled,
			DoubleBuffered,
		};
	}
	using VSyncMode = VSyncModes::Enum;
}

#endif