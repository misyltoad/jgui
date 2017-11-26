#include <jgui_private/globals.h>

namespace jgui
{
	namespace globals
	{
		Renderer renderer = Renderers::Null;
		VSyncMode vsync = VSyncModes::DoubleBuffered;
		std::vector<FontData> fontData;
	}
}