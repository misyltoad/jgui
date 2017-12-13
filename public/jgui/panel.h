#ifndef JGUI_PANEL_H
#define JGUI_PANEL_H

#include <vector>

#include <jgui/jgui_common.h>
#include <jgui/bounds.h>
#include <jgui/renderable.h>
#include <jgui/data_desc.h>
#include <jgui/time.h>

namespace jgui
{
	class Window;
	class Panel;

	class jgui_export Panel : public Renderable
	{
	public:
		DECLARE_CLASS(Panel, Renderable);
		DECLARE_DATA_DESC();

		Panel();
		virtual ~Panel();
		static Panel* FindPanel(const char* name);

		Panel* GetParent();
		void SetParent(Panel* parent);
		void SetParentByName(const char* parentName);

		virtual void OnStartHover();
		virtual void OnEndHover();
		virtual void OnClick();

		virtual void OnNameUpdated();
		void SetName(const char* name);
		const char* GetName() const;

		virtual void Paint();
		void Render(u32 xOffset, u32 yOffset);

		virtual void OnCreated();

		Bounds& GetBounds();

		virtual void OnBoundsUpdated(bool pos, bool wide);

		void SetBounds(const Bounds& bounds);
		void SetBounds(const char* data);
		void SetBounds(u32 x, u32 y, u32 width, u32 height);

		void SetSize(const char* data);
		void SetSize(u32 width, u32 height);
		void SetPosition(const char* data);
		void SetPosition(u32 x, u32 y);

		void RecomputeScale();

		Window* GetWindow();

		virtual void Think(f32 time, f32 dt);

		void SetMaintainAspectRatio(bool maintainAspectRatio);
		DESC_FUNC_BOOL_WRAPPER(SetMaintainAspectRatio);

		void SetRoundness(f32 roundness);
		DESC_FUNC_FLOAT_WRAPPER(SetRoundness)

		void SetBackgroundColour(Colour colour);
		DESC_FUNC_COLOUR_WRAPPER(SetBackgroundColour);

	protected:
		virtual void* GetRenderContext();
		void AddChild(Panel* panel);
		void RemoveChild(Panel* panel);

		bool HoveredOn;
		bool ClickedOn;
		bool MaintainAspectRatio;

		friend void RenderAllWindows();
		void RealThink(f32 time, f32 dt);

		Bounds Bounds;
		Panel* Parent;
		void* RenderContext;
		const char* Name;
		Colour BackgroundColour;
		f32 Roundness;
		std::vector<Panel*>* Children;
	};
}

#endif