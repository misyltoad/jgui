#ifndef JGUI_GLOBALS_H
#define JGUI_GLOBALS_H

#include <jgui/renderers.h>
#include <jgui/vsync_modes.h>
#include <jgui/window.h>
#include <GLFW/glfw3.h>
#include <bgfx/bgfx.h>
#include <nanovg.h>

namespace jgui
{
	struct WindowData
	{
		Window* jguiPtr;
		GLFWwindow* glfwPtr;
		void* nativeHandle;
		bgfx::FrameBufferHandle fbh;
		NVGcontext* vg;
		bool dirtyFb;
	};

	struct FontData
	{
		std::string Name;
		u8* Data;
		u32 Size;
	};


	namespace globals
	{
		extern Renderer renderer;
		extern VSyncMode vsync;

		extern std::vector<WindowData> ActiveWindows;
		extern std::vector<FontData> fontData;
	}
}

#endif