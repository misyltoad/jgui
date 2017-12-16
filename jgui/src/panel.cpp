#include <jgui/panel.h>
#include <jgui/colour.h>
#include <jgui/window.h>
#include <unordered_map>
#include <jgui_private/string_helpers.h>
#include <jgui/data_desc.h>
#include <jgui/command.h>

namespace jgui
{
	START_DATA_DESC_NO_DESC_PARENT(Panel)
		DATA_DESC_DATA_FUNC(Name, Panel::SetName)
		DATA_DESC_DATA_FUNC(Parent, Panel::SetParentByName)
		DATA_DESC_DATA_FUNC(Size, Panel::SetSize)
		DATA_DESC_DATA_FUNC(Position, Panel::SetPosition)
		DATA_DESC_DATA_FUNC(Pos, Panel::SetPosition)
		DATA_DESC_DATA_FUNC(Bounds, Panel::SetBounds)
		DATA_DESC_DATA_FUNC(BackgroundColour, Panel::SetBackgroundColour)
		DATA_DESC_DATA_FUNC(BackgroundColor, Panel::SetBackgroundColour)
		DATA_DESC_DATA_FUNC(Roundness, Panel::SetRoundness)
		DATA_DESC_DATA_FUNC(MaintainAspectRatio, Panel::SetMaintainAspectRatio)
		DATA_DESC_DATA_FUNC(Visible, Panel::SetVisible)
	END_DATA_DESC(Panel)

	std::unordered_map<std::string, Panel*> Panels;

	Panel::Panel()
		: m_Parent(nullptr)
		, m_Bounds({})
		, m_RenderContext(nullptr)
		, m_Roundness(0.0f)
		, m_BackgroundColour(0, 0, 0, 0)
		, m_MaintainAspectRatio(false)
		, m_Visible(true)
		, m_Children()
	{
		scaleX = 1.0f;
		scaleY = 1.0f;
	}
	Panel::~Panel()
	{
		if (!m_Name.empty())
			Panels.erase(m_Name);

		if (m_Parent)
			m_Parent->RemoveChild(this);
	}
	Panel* Panel::FindPanel(const char* name)
	{
		auto parentIndex = Panels.find(name);
		if (parentIndex != Panels.end())
			return Panels[name];
		else
			return nullptr;
	}
	Panel* Panel::GetParent()
	{
		return m_Parent;
	}
	void Panel::SetParent(Panel* parent, bool tellParent)
	{
		if (m_Parent && tellParent)
			m_Parent->RemoveChild(this);

		m_Parent = parent;
		if (parent)
		{
			if (tellParent)
				parent->AddChild(this);
			m_RenderContext = parent->GetRenderContext();
		}

		RecomputeScale();
	}
	void Panel::SetParentByName(const char* parentName)
	{
		SetParent(FindPanel(parentName));
	}
	void Panel::OnStartHover()
	{
	}
	void Panel::OnEndHover()
	{
	}

	QUICK_MEMBER_IMPLEMENT_STRING(Panel, Name)
	{
		if (m_Name == value)
			return;

		if (!m_Name.empty())
			Panels.erase(m_Name);

		m_Name = "";

		if (value.empty())
		{
			OnNameUpdated();
			return;
		}

		m_Name = value;
		Panels[m_Name] = this;
		OnNameUpdated();
	}

	QUICK_MEMBER_IMPLEMENT_BOOL(Panel, MaintainAspectRatio)
	{
		m_MaintainAspectRatio = value;
		RecomputeScale();
	}

	QUICK_MEMBER_IMPLEMENT_BOOL_SIMPLE(Panel, Visible)
	QUICK_MEMBER_IMPLEMENT_FLOAT_SIMPLE(Panel, Roundness)
	QUICK_MEMBER_IMPLEMENT_COLOUR_SIMPLE(Panel, BackgroundColour)

	void Panel::OnClick()
	{
		//ExecuteCommand(ClickCommand);
	}
	void Panel::OnNameUpdated()
	{
	}
	void Panel::Paint()
	{
		auto& bounds = GetBounds();
		BeginPath();
		FillColour(m_BackgroundColour);
		AddRect((f32)bounds.x, (f32)bounds.y, (f32)bounds.width, (f32)bounds.height, m_Roundness);
		FillPath();
	}
	void Panel::Render(f32 xOffset, f32 yOffset)
	{
		if (!m_Visible)
			return;

		this->xOffset = xOffset;
		this->yOffset = yOffset;

		Paint();

		for (usize i = 0; i < m_Children.size(); i++)
			m_Children[i]->Render(xOffset + m_Bounds.x, yOffset + m_Bounds.y);
	}
	void Panel::OnCreated()
	{
	}
	Bounds& Panel::GetBounds()
	{
		return m_Bounds;
	}
	void Panel::OnBoundsUpdated(bool pos, bool wide)
	{
	}
	void Panel::SetBounds(const jgui::Bounds& bounds)
	{
		m_Bounds = bounds;
		OnBoundsUpdated(true, true);
	}
	void Panel::SetBounds(const char* data)
	{
		auto list = comma_list_to_numbers(data);
		if (list.size() == 4)
			SetBounds(list[0], list[1], list[2], list[3]);
	}
	void Panel::SetBounds(u32 x, u32 y, u32 width, u32 height)
	{
		m_Bounds.x = x;
		m_Bounds.y = y;
		m_Bounds.width = width;
		m_Bounds.height = height;
		OnBoundsUpdated(true, true);
	}
	void Panel::SetSize(const char* data)
	{
		auto list = comma_list_to_numbers(data);
		if (list.size() == 2)
			SetSize(list[0], list[1]);
	}
	void Panel::SetSize(u32 width, u32 height)
	{
		m_Bounds.width = width;
		m_Bounds.height = height;
		OnBoundsUpdated(false, true);
	}
	void Panel::SetPosition(const char* data)
	{
		auto list = comma_list_to_numbers(data);
		if (list.size() == 2)
			SetPosition(list[0], list[1]);
	}
	void Panel::SetPosition(u32 x, u32 y)
	{
		m_Bounds.x = x;
		m_Bounds.y = y;
		OnBoundsUpdated(true, false);
	}
	void Panel::RecomputeScale()
	{
		if (GetWindow() && GetWindow()->GetIsProportional())
		{
			auto bounds = GetWindow()->GetBounds();
			scaleX = (bounds.width) / 640.0f;
			scaleY = (bounds.height) / 480.0f;
			if (m_MaintainAspectRatio)
			{
				scaleX = MIN(scaleX, scaleY);
				scaleY = scaleX;
			}
		}
		else
		{
			scaleX = 1.0f;
			scaleY = 1.0f;
		}

		for (int i = 0; i < m_Children.size(); i++)
			m_Children[i]->RecomputeScale();
	}
	Window* Panel::GetWindow()
	{
		Panel* parent = this;

		for (;;)
		{
			if (parent)
			{
				Window* window = dynamic_cast<Window*>(parent);
				if (window)
					return window;

				parent = parent->GetParent();
				if (parent == this)
					return nullptr;
			}
			else
				return nullptr;
		}
	}
	void Panel::Think(f32 time, f32 dt)
	{
		
	}
	void* Panel::GetRenderContext()
	{
		return m_RenderContext;
	}
	void Panel::AddChild(Panel* panel)
	{
		m_Children.push_back(panel);
		panel->SetParent(this, false);
	}
	void Panel::RemoveChild(Panel* panel)
	{
		if (panel->GetParent() == this)
			panel->SetParent(nullptr, false);

		for (usize i = 0; i < m_Children.size(); i++)
		{
			if (m_Children[i] == panel)
				m_Children.erase(m_Children.begin() + i);
		}
	}
	const std::vector<Panel*>& Panel::GetChildren()
	{
		return m_Children;
	}
	void Panel::RealThink(f32 time, f32 dt)
	{
		Think(time, dt);

		for (usize i = 0; i < m_Children.size(); i++)
			m_Children[i]->Think(time, dt);
	}
}