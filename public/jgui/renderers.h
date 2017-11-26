#ifndef JGUI_RENDERERS_H
#define JGUI_RENDERERS_H

namespace jgui
{
	namespace Renderers
	{
		enum Enum
		{
			Null,
			OpenGL,
			DirectX9,
			DirectX11,
		};
	}
	using Renderer = Renderers::Enum;
}

#endif