#ifndef JGUI_INIT_H
#define JGUI_INIT_H

#include <jgui/renderers.h>
#include <jgui/jgui_common.h>
#include <jgui/vsync_modes.h>

namespace jgui
{
	bool jgui_export init(Renderer renderer = Renderers::DirectX11, VSyncMode vsync = VSyncModes::DoubleBuffered);
	void jgui_export update();
	void jgui_export shutdown();
}

#endif