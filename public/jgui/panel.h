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
		void SetParent(Panel* parent, bool tellParent = true);
		void SetParentByName(const char* parentName);

		virtual void OnStartHover();
		virtual void OnEndHover();
		virtual void OnClick();

		virtual void OnNameUpdated();
		QUICK_MEMBER_STRING(Name);

		virtual void Paint();
		virtual void Render(f32 xOffset, f32 yOffset);

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

		virtual void RecomputeScale();

		Window* GetWindow();

		virtual void Think(f32 time, f32 dt);

		QUICK_MEMBER_BOOL(MaintainAspectRatio);

		QUICK_MEMBER_FLOAT(Roundness);

		QUICK_MEMBER_BOOL(Visible);

		QUICK_MEMBER_COLOUR(BackgroundColour);

		void AddChild(Panel* panel);
		void RemoveChild(Panel* panel);
		const std::vector<Panel*>& GetChildren();
	protected:
		virtual void* GetRenderContext();

	private:
		friend void RenderAllWindows();
		void RealThink(f32 time, f32 dt);

		Bounds m_Bounds;
		Panel* m_Parent;
		void* m_RenderContext;

		std::vector<Panel*> m_Children;
	};
}

#endif