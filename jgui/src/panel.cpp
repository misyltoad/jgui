#include <jgui/panel.h>
#include <jgui/colour.h>
#include <jgui/window.h>
#include <unordered_map>
#include <jgui_private/string_helpers.h>
#include <jgui/data_desc.h>

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
	END_DATA_DESC(Panel)

	std::unordered_map<std::string, Panel*> Panels;

	Panel::Panel()
		: Parent(nullptr)
		, Bounds({})
		, HoveredOn(false)
		, ClickedOn(false)
		, RenderContext(nullptr)
		, Name(nullptr)
		, Roundness(0.0f)
		, BackgroundColour(0, 0, 0, 0)
		, MaintainAspectRatio(false)
		, Children(new std::vector<Panel*>())
	{
		scaleX = 1.0f;
		scaleY = 1.0f;
	}
	Panel::~Panel()
	{
		if (Name)
		{
			Panels.erase(Name);
			free((void*)Name);
		}

		if (Parent)
			Parent->RemoveChild(this);
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
		return Parent;
	}
	void Panel::SetParent(Panel* parent)
	{
		if (Parent)
			Parent->RemoveChild(this);

		Parent = parent;
		if (parent)
		{
			parent->AddChild(this);
			RenderContext = parent->GetRenderContext();
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
	void Panel::OnClick()
	{
	}
	void Panel::OnNameUpdated()
	{
	}
	void Panel::SetName(const char* name)
	{
		if (name == Name)
			return;

		if (Name)
		{
			Panels.erase(Name);
			free((void*)Name);
			Name = nullptr;
		}

		if (name == nullptr)
		{
			OnNameUpdated();
			return;
		}

		usize sizeRequired = strlen(name) + 1;
		char* allocName = (char*) malloc(sizeRequired);
		if (allocName)
		{
			memcpy(allocName, name, sizeRequired);
			Name = allocName;
		}

		Panels[Name] = this;

		OnNameUpdated();
	}
	const char* Panel::GetName() const
	{
		return Name;
	}
	void Panel::Paint()
	{
		auto& bounds = GetBounds();
		BeginPath();
		FillColour(BackgroundColour);
		AddRect((f32)bounds.x, (f32)bounds.y, (f32)bounds.width, (f32)bounds.height, Roundness);
		FillPath();
	}
	void Panel::Render(u32 xOffset, u32 yOffset)
	{
		this->xOffset = xOffset;
		this->yOffset = yOffset;

		Paint();

		for (usize i = 0; i < Children->size(); i++)
			(*Children)[i]->Render(xOffset + Bounds.x, yOffset + Bounds.y);
	}
	void Panel::OnCreated()
	{
	}
	Bounds& Panel::GetBounds()
	{
		return Bounds;
	}
	void Panel::OnBoundsUpdated(bool pos, bool wide)
	{
	}
	void Panel::SetBounds(const jgui::Bounds& bounds)
	{
		Bounds = bounds;
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
		Bounds.x = x;
		Bounds.y = y;
		Bounds.width = width;
		Bounds.height = height;
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
		Bounds.width = width;
		Bounds.height = height;
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
		Bounds.x = x;
		Bounds.y = y;
		OnBoundsUpdated(true, false);
	}
	void Panel::RecomputeScale()
	{

		if (GetWindow() && GetWindow()->GetIsProportional())
		{
			auto bounds = GetWindow()->GetBounds();
			scaleX = (bounds.width) / 640.0f;
			scaleY = (bounds.height) / 480.0f;
			if (MaintainAspectRatio)
			{
				if (scaleX > scaleY * 1.5)
					scaleX = scaleY;
				else
					scaleY = scaleX;
			}
		}
		else
		{
			scaleX = 1.0f;
			scaleY = 1.0f;
		}

		for (int i = 0; i < Children->size(); i++)
			(*Children)[i]->RecomputeScale();
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
	void Panel::SetMaintainAspectRatio(bool maintainAspectRatio)
	{
		MaintainAspectRatio = maintainAspectRatio;
		RecomputeScale();
	}
	void Panel::SetRoundness(f32 roundness)
	{
		Roundness = roundness;
	}
	void Panel::SetBackgroundColour(Colour colour)
	{
		BackgroundColour = colour;
	}
	void* Panel::GetRenderContext()
	{
		return RenderContext;
	}
	void Panel::AddChild(Panel* panel)
	{
		Children->push_back(panel);
	}
	void Panel::RemoveChild(Panel* panel)
	{
		for (usize i = 0; i < Children->size(); i++)
		{
			if ((*Children)[i] == panel)
				Children->erase(Children->begin() + i);
		}
	}
	void Panel::RealThink(f32 time, f32 dt)
	{
		Think(time, dt);

		for (usize i = 0; i < Children->size(); i++)
			(*Children)[i]->Think(time, dt);
	}
}