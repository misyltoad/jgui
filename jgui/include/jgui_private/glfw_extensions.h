#ifndef JGUI_GLFW_EXTENSIONS_H
#define JGUI_GLFW_EXTENSIONS_H

#include <bx/platform.h>

#if BX_PLATFORM_LINUX || BX_PLATFORM_BSD
#define GLFW_EXPOSE_NATIVE_X11
#define GLFW_EXPOSE_NATIVE_GLX
#elif BX_PLATFORM_OSX
#define GLFW_EXPOSE_NATIVE_COCOA
#define GLFW_EXPOSE_NATIVE_NSGL
#elif BX_PLATFORM_WINDOWS
#define GLFW_EXPOSE_NATIVE_WIN32
#define GLFW_EXPOSE_NATIVE_WGL
#endif

#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

extern "C"
{
	inline void* glfwNativeWindowHandle(GLFWwindow* window)
	{
#if BX_PLATFORM_LINUX || BX_PLATFORM_BSD
		return (void*)(uintptr_t)glfwGetX11Window(window);
#elif BX_PLATFORM_OSX
		return glfwGetCocoaWindow(window);
#elif BX_PLATFORM_WINDOWS
		return glfwGetWin32Window(window);
#endif
	}
}

#endif