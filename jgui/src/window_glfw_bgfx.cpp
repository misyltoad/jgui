#if defined(JGUI_USE_GLFW) && defined(JGUI_USE_BGFX)

#include <vector>
#include <jgui/window.h>
#include <GLFW/glfw3.h>
#include <jgui_private/glfw_extensions.h>
#include <bgfx/bgfx.h>
#include <bgfx/platform.h>
#include <algorithm>
#include <jgui_private/globals.h>
#include <jgui_private/fonts/Roboto-Regular.ttf.h>
#include <jgui_private/globals.h>

#include <nanovg.h>
#include <nanovg_bgfx.h>

#define DEFAULT_WINDOW_TITLE "jgui Window"

namespace jgui
{
	namespace globals
	{
		std::vector<WindowData> ActiveWindows;
	}

	START_DATA_DESC(Window)
		DATA_DESC_DATA_FUNC(IsProportional, Window::SetIsProportional)
		DATA_DESC_DATA_FUNC(Proportional, Window::SetIsProportional)
	END_DATA_DESC(Window)

	WindowData* GetWindowData(Window* window)
	{
		for (usize i = 0; i < globals::ActiveWindows.size(); i++)
		{
			if (globals::ActiveWindows[i].jguiPtr == window)
				return &globals::ActiveWindows[i];
		}
		return nullptr;
	}

	WindowData* GetWindowData(GLFWwindow* window)
	{
		for (usize i = 0; i < globals::ActiveWindows.size(); i++)
		{
			if (globals::ActiveWindows[i].glfwPtr == window)
				return &globals::ActiveWindows[i];
		}
		return nullptr;
	}

	void RegenFB(Window* window)
	{
		WindowData* data = GetWindowData(window);
		if (data)
		{
			int w, h;
			glfwGetWindowSize(data->glfwPtr, &w, &h);
			window->SetSize(w, h);

			window->RecomputeScale();
			if (bgfx::isValid(data->fbh))
				bgfx::destroy(data->fbh);

			data->fbh = bgfx::createFrameBuffer(data->nativeHandle, w, h);
			data->dirtyFb = false;
		}
	}

	void PositionCallback(GLFWwindow* window, int x, int y)
	{
		WindowData* data = GetWindowData(window);
		if (data)
			data->jguiPtr->SetPosition(x, y);
	}

	void QuitCallback(GLFWwindow* window)
	{
		WindowData* data = GetWindowData(window);
		if (data)
			data->jguiPtr->OnQuitRequested();
	}

	void ResizeCallback(GLFWwindow* window, int width, int height)
	{
		WindowData* data = GetWindowData(window);
		if (data)
			data->dirtyFb = true;
	}

	void UpdateWindow(Window* window)
	{
		WindowData* data = GetWindowData(window);

		if (data)
		{
			if (data->dirtyFb)
				RegenFB(window);

			glfwPollEvents();
			//glfwSwapBuffers(data->glfwPtr);
		}
	}

	void RenderAllWindows()
	{
		static Time firstTime = Time::Now();
		static Time lastTime = Time::Now();
		Time currentTime = Time::Now();
		Time difference = currentTime - lastTime;
		f32 dt = difference.AsSeconds();
		f32 time = (currentTime - firstTime).AsSeconds();
		lastTime = currentTime;

		usize sizeBefore = globals::ActiveWindows.size();
		if (sizeBefore > 0)
		{
			for (usize i = 0; i < sizeBefore; i++)
			{
				bgfx::touch((u8)i);

				WindowData* data = &globals::ActiveWindows[i];

				UpdateWindow(globals::ActiveWindows[i].jguiPtr);

				usize sizeAfter = globals::ActiveWindows.size();

				if (sizeBefore != sizeAfter)
				{
					// Early out to avoid running stuff on any possibly dead windows.
					return;
				}

				//if (!bgfx::isValid(data->fbh))
				//	RegenFB(data->jguiPtr, data->width, data->height);

				bgfx::setViewFrameBuffer((u8)i, data->fbh);

				auto Bounds = data->jguiPtr->GetBounds();

				bgfx::setViewRect((u8)i, 0, 0, Bounds.width, Bounds.height);

				bgfx::setViewClear((u8)i
					, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH | BGFX_CLEAR_STENCIL
					, 0x00000000
					, 1.0f
					, 0
				);

				nvgBeginFrame(data->vg, Bounds.width, Bounds.height, 1.0f);

				data->jguiPtr->RealThink(time, dt);
				data->jguiPtr->Render(0, 0);
				nvgEndFrame(data->vg);
			}

			bgfx::frame();
		}
	}

	Window::Window()
	{
		m_IsProportional = false;
	}

	Window::~Window()
	{
		Shutdown();
	}

	bool Window::Open()
	{
		if (globals::ActiveWindows.size() >= 255)
			return false;

		//glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
		//glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

		std::string name = GetName();

		GLFWwindow* window = glfwCreateWindow(static_cast<int>(Bounds.width), static_cast<int>(Bounds.height), name.empty() ? DEFAULT_WINDOW_TITLE : name.c_str(), nullptr, nullptr);

		GLFWmonitor* monitor = glfwGetPrimaryMonitor();

		if (monitor)
		{
			const GLFWvidmode* desktop = glfwGetVideoMode(monitor);
			if (desktop)
				glfwSetWindowPos(window, (desktop->width - Bounds.width) / 2, (desktop->height - Bounds.height) / 2);
		}

		if (!window)
			return false;

		if (globals::ActiveWindows.empty())
		{
			bgfx::PlatformData pd;

#if BX_PLATFORM_LINUX || BX_PLATFORM_BSD
			pd.ndt = glfwGetX11Display();
#elif BX_PLATFORM_OSX
			pd.ndt = NULL;
#elif BX_PLATFORM_WINDOWS
			pd.ndt = NULL;
#endif

			pd.nwh = glfwNativeWindowHandle(window);
			pd.context = NULL;
			pd.backBuffer = NULL;
			pd.backBufferDS = NULL;
			bgfx::setPlatformData(pd);

			bgfx::renderFrame();

			bgfx::RendererType::Enum renderer;

			switch (globals::renderer)
			{
			case Renderers::OpenGL: renderer = bgfx::RendererType::OpenGL; break;
			case Renderers::DirectX9: renderer = bgfx::RendererType::Direct3D9; break;
			case Renderers::DirectX11: renderer = bgfx::RendererType::Direct3D11; break;
			default:
			case Renderers::Null: renderer = bgfx::RendererType::Noop; break;
			}

			if (!bgfx::init(renderer))
				return false;

			bgfx::reset(Bounds.width, Bounds.height, globals::vsync == VSyncModes::DoubleBuffered ? BGFX_RESET_VSYNC : 0);

			//bgfx::setDebug(BGFX_DEBUG_STATS);
		}

		if (Bounds.x != Bounds::InvalidPosition && Bounds.y != Bounds::InvalidPosition)
			glfwSetWindowPos(window, Bounds.x, Bounds.y);

		glfwSetWindowSizeCallback(window, ResizeCallback);
		glfwSetWindowPosCallback(window, PositionCallback);
		glfwSetWindowCloseCallback(window, QuitCallback);

		WindowData data = {};
		data.jguiPtr = this;
		data.glfwPtr = window;

		data.nativeHandle = glfwNativeWindowHandle(window);
		data.fbh = BGFX_INVALID_HANDLE;
		data.vg = nvgCreate(1, (u8)globals::ActiveWindows.size());

		for (usize i = 0; i < globals::fontData.size(); i++)
			nvgCreateFontMem(data.vg, globals::fontData[i].Name.c_str(), globals::fontData[i].Data, globals::fontData[i].Size, 0);

		globals::ActiveWindows.push_back(data);

		RegenFB(this);

		return true;
	}

	void Window::Shutdown()
	{
		for (usize i = 0; i < globals::ActiveWindows.size(); i++)
		{
			if (globals::ActiveWindows[i].jguiPtr == this)
			{
				if (bgfx::isValid(globals::ActiveWindows[i].fbh))
					bgfx::destroy(globals::ActiveWindows[i].fbh);

				if (globals::ActiveWindows.size() == 1)
					bgfx::shutdown();

				glfwDestroyWindow(globals::ActiveWindows[i].glfwPtr);
				globals::ActiveWindows.erase(globals::ActiveWindows.begin() + i);
			}
		}
	}
	bool Window::AnyOpen()
	{
		return globals::ActiveWindows.size() > 0;
	}
	void Window::OnQuitRequested()
	{
		Shutdown();
	}
	void Window::OnNameUpdated()
	{
		auto* data = GetWindowData(this);
		if (data)
		{
			std::string name = GetName();
			glfwSetWindowTitle(data->glfwPtr, name.empty() ? DEFAULT_WINDOW_TITLE : name.c_str());
		}
	}
	void Window::OnBoundsUpdated(bool pos, bool wide)
	{
		auto* data = GetWindowData(this);
		if (data)
		{
			int x, y, w, h;
			glfwGetWindowPos(data->glfwPtr, &x, &y);
			glfwGetWindowSize(data->glfwPtr, &w, &h);

			if (pos)
			{
				if (x != Bounds.x || Bounds.y != y)
					glfwSetWindowPos(data->glfwPtr, Bounds.x, Bounds.y);
			}

			if (wide)
			{
				if (w != Bounds.width || h != Bounds.height)
					glfwSetWindowSize(data->glfwPtr, Bounds.width, Bounds.height);
			}

			RecomputeScale();
		}
	}
	void Window::Render(u32 xOffset, u32 yOffset)
	{
		this->xOffset = 0;
		this->yOffset = 0;

		Paint();

		for (usize i = 0; i < Children.size(); i++)
			Children[i]->Render(0, 0);
	}
	void Window::OnCreated()
	{
		Open();
	}
	void* Window::GetRenderContext()
	{
		auto* data = GetWindowData(this);
		if (data)
			return data->vg;

		return nullptr;
	}

	QUICK_MEMBER_IMPLEMENT_BOOL(Window, IsProportional)
	{
		m_IsProportional = value;
		RecomputeScale();
	}
}

#endif