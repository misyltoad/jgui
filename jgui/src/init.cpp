#include <jgui/init.h>

#ifdef JGUI_USE_GLFW
#include <GLFW/glfw3.h>
#endif

#ifdef JGUI_USE_BGFX
#include <bgfx/bgfx.h>
#endif

#include <jgui_private/globals.h>
#include <jgui/font.h>
#include <jgui_private/fonts/Roboto-Regular.ttf.h>

#include <stdio.h>

namespace jgui
{
	bool init(Renderer renderer, VSyncMode vsync)
	{
		globals::renderer = renderer;
		globals::vsync = vsync;

		register_font("default", (unsigned char*)Roboto_Regular_ttf_data, Roboto_Regular_ttf_size);

#ifdef JGUI_USE_GLFW
		if (glfwInit() != GLFW_TRUE)
			return false;
#endif
		return true;
	}

	extern void RenderAllWindows();

	void render()
	{
		RenderAllWindows();
	}

	void shutdown()
	{
#ifdef JGUI_USE_GLFW
		glfwTerminate();
#endif
	}
}
