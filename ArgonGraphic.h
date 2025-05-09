#pragma once
#include <typeindex>

// ----------------------------------------------------------- //
//  [ARGON] [ArgonGraphic] Enum class declarations
// ----------------------------------------------------------- //

// [FLAGS] [ArgonGraphicSystem] - To describe the state of the element.
enum class ArGraphicElementFlag : uint32_t
{
	Enable = 1 << 0,
	Dead = 1 << 1,
	Visible = 1 << 2,
	Interactive = 1 << 3,
	Focusable = 1 << 4,
	Debug = 1 << 5,
	None = 0
};

// [FLAGS] [ArgonGraphicSystem] - To describe the callback state of the element.
enum class ArGraphicElementBeingState : uint32_t
{
	Focus = 1 << 0,
	Unfocus = 1 << 1,
	Hover = 1 << 2,
	Unhover = 1 << 3,
	ChildAdded = 1 << 4,
	ChildRemoved = 1 << 5,
	All = Focus | Unfocus | Hover | Unhover | ChildAdded | ChildRemoved,
	None = 0,
};

// [ENUM] [ArgonGraphicSystem] - To describe the priority of the layer addition.
enum class ArLayerAdditionPriority
{
	BeforeDefault,
	AfterDefault,
	AfterBackground,
	BeforeForeground,
	None
};

// [ENUM] [ArgonGraphicSystem] - To describe the mode of the timer.
enum class ArTimerCompMode
{
	Once,
	Loop,
	None
};

// [ENUM] [ArgonGraphicSystem] - To describe the direction of the flex layout.
enum class ArFlexLayoutDirection
{
	Horizontal,
	Vertical,
	None
};

// [ENUM] [ArgonGraphicSystem] - To describe the justify content of the flex layout.
enum class ArFlexLayoutJustifyContent
{
	FlexStart,
	FlexEnd,
	Center,
	SpaceBetween,
	SpaceAround,
	SpaceEvenly,
	None
};

// [ENUM] [ArgonGraphicSystem] - To describe the align items of the flex layout.
enum class ArFlexLayoutAlignItems
{
	FlexStart,
	FlexEnd,
	Center,
	Stretch,
	None
};

// [ENUM] [ArgonGraphicSystem] - To describe the align self of the flex layout.
enum class ArFlexLayoutWrap
{
	NoWrap,
	Wrap,
	None
};

// [ENUM] [ArgonGraphicSystem] - To describe the align content of the flex layout.
enum class ArFlexLayoutAlignContent
{
	FlexStart,
	FlexEnd,
	Center,
	Stretch,
	SpaceBetween,
	SpaceAround,
	SpaceEvenly,
	None
};


// ----------------------------------------------------------- //
//  [ARGON] [ArgonGraphic] Interface declarations
// ----------------------------------------------------------- //

// [INTERFACE] [ArgonGraphicSystem] - Graphic component interface. This interface is used to create custom components for the graphic system.
class IArGraphicElementComponent
{
public:
	virtual void Awake(ArGraphicElement& owner) {};
	virtual void OnUpdate(ArGraphicElement& owner, ArgonContext& context) {};
	virtual void OnRender(ArGraphicElement& owner) {};
	virtual void OnChildrenListChanged(ArGraphicElement& owner, ArgonContext& context) {};
};

// ----------------------------------------------------------- //
//  [ARGON] [ArgonGraphic] Base element
// ----------------------------------------------------------- //

// [CLASS] [ArgonGraphicSystem] - Render list with owner element.
class ArGraphicRenderList final : public ArRenderList
{
public:
	ArGraphicElement* owner = nullptr;
public:
	ArGraphicRenderList(ArRenderListSharedData* sharedData, ArGraphicElement* owner) : ArRenderList(sharedData), owner(owner) {}
};

// [STRUCT] [ArgonGraphicSystem] - Bounding box of the element.
class ArGraphicBoxModel final
{
public:
	class Padding final
	{
	private:
		ArVec4 value = ArVec4();
	public:
		void Set(float top, float right, float bottom, float left) { value = ArVec4(top, right, bottom, left); }

		void Set(float all) { value = ArVec4(all, all, all, all); }

		void Set(float x, float y) { value = ArVec4(y, x, y, x); }

		void SetLeft(float left) { value = ArVec4(value.x, left, value.z, value.w); }

		void SetRight(float right) { value = ArVec4(value.x, value.y, value.z, right); }

		void SetTop(float top) { value = ArVec4(top, value.y, value.z, value.w); }

		void SetBottom(float bottom) { value = ArVec4(value.x, value.y, bottom, value.w); }

		ArVec2 Size() const { return ArVec2(value.y + value.z, value.x + value.w); }

		ArVec2 TopLeft() const { return ArVec2(value.y, value.x); }

		ArVec2 BottomRight() const { return ArVec2(value.z, value.w); }

		float Top() const { return value.x; }

		float Bottom() const { return value.w; }

		float Left() const { return value.y; }

		float Right() const { return value.z; }

		ArVec4* operator->() { return &value; }

		const ArVec4* operator->() const { return &value; }

		operator ArVec4() { return value; }

		operator const ArVec4() const { return value; }
	};
	using Margin = Padding;
public:
	ArVec2 position = ArVec2();			// Position to padding rect top left
	ArVec2 localPosition = ArVec2();	// Offset to origin of the owner element

	Margin margin = Margin();
	Padding padding = Padding();
	ArVec2 content = ArVec2();
	float borderSize = 0.f;

	bool overflow = false;
public:
	ArRect GetContentRect() const { return ArRect(position + localPosition, position + localPosition + content); }

	ArRect GetBorderRect() const { ArRect baseRect = GetContentRect(); return ArRect(baseRect.GetTL() - borderSize, baseRect.GetBR() + borderSize); }

	ArRect GetPaddingRect() const { ArRect baseRect = GetBorderRect(); return ArRect(baseRect.GetTL() - padding.TopLeft(), baseRect.GetBR() + padding.BottomRight()); }

	ArRect GetMarginRect() const { ArRect baseRect = GetPaddingRect(); return ArRect(baseRect.GetTL() - margin.TopLeft(), baseRect.GetBR() + margin.BottomRight()); }

	ArVec2 GetContentPosition() const { return position + localPosition + padding.TopLeft(); }

	ArVec2 GetBorderBoxSize() const { return content + borderSize + padding.Size(); }

	ArVec2 GetContentSize() const { return content; }


	bool operator==(const ArGraphicBoxModel& other) const { return position == other.position && localPosition == other.localPosition && margin == other.margin && padding == other.padding && content == other.content && overflow == other.overflow; }

	bool operator!=(const ArGraphicBoxModel& other) const { return !(*this == other); }
};

// [CLASS] [ArgonGraphicSystem] - Basic element of the graphic system.
class ArGraphicElement
{
public:
	ArGraphicLayer* layer = nullptr;
	ArGraphicElement* owner = nullptr;
	ArGraphicElement* root = nullptr;

	ArGraphicBoxModel borderBox = ArGraphicBoxModel();
	ArGraphicRenderList* renderList = nullptr;

	int depth = 0;
	bool focusing = false;
	bool hovering = false;

	ArGraphicElementFlag flags = ArGraphicElementFlag::Enable | ArGraphicElementFlag::Visible | ArGraphicElementFlag::Focusable | ArGraphicElementFlag::Interactive;
private:
	friend class ArgonGraphicManager;
	friend class ArGraphicLayer;
	ArGraphicElementBeingState beingState = ArGraphicElementBeingState::None;

	std::deque<ArGraphicElement*> unwakedChildren = {};
	std::vector<ArGraphicElement*> children = {};
	std::unordered_map<std::type_index, std::vector<IArGraphicElementComponent*>> componentMap = {};
public:
	ArGraphicElement() {}
	ArGraphicElement(ArGraphicElement* owner) : owner(owner) {}
	ArGraphicElement(ArGraphicElementFlag defaultFlags) : flags(defaultFlags) {}
	ArGraphicElement(const ArGraphicElement&) = delete;
	ArGraphicElement& operator=(const ArGraphicElement&) = delete;
	virtual ~ArGraphicElement();


	void Awaken(ArgonContext& context);

	void FrameUpdate(ArgonContext& context);


	ArGraphicElement* GetRootElement();

	int FindMostDeepestChild() const;

	template<typename T>
	T* AddComponent(T* component) { componentMap[typeid(T)].push_back(component); return component; }

	template<typename T>
	bool HasComponent() const { return componentMap.find(typeid(T)) != componentMap.end(); }

	template<typename T>
	T* GetComponent(size_t index = 0) const { auto it = componentMap.find(typeid(T)); if (it == componentMap.end() || index >= it->second.size()) return nullptr; return (T*)it->second[index]; }

	void AddChild(ArGraphicElement* child);

	size_t GetChildCount() const { return children.size(); }

	ArGraphicElement* GetChild(size_t index) const;

	const std::vector<ArGraphicElement*>& GetChildren() const { return children; }

	std::vector<ArGraphicElement*> GetAvailableChildren() const;

	bool IsDead() const { return ArHasFlag(flags, ArGraphicElementFlag::Dead); }

	void Kill();


	virtual void Awake(ArgonContext& context) {}

	virtual void OnUpdate(ArgonContext& context) {}

	virtual void OnHover(ArgonContext& context) {}

	virtual void OnUnhover(ArgonContext& context) {}

	virtual void OnFocus(ArgonContext& context) {}

	virtual void OnFocusLost(ArgonContext& context) {}

	virtual void OnRender(ArgonContext& context) {}

	virtual bool HitTest(const ArVec2& pos) const { return borderBox.GetContentRect().Contains(pos); }
};

// [CLASS] [ArgonGraphicSystem] - Layer of the graphic system.
class ArGraphicLayer final
{
private:
	std::queue<ArGraphicElement*> unwakedElements = {};
	std::vector<ArGraphicElement*> elements = {};
public:
	bool lockFocus = false;
	ArGraphicElement* focusingElement = nullptr;
	ArGraphicElement* hoveringElement = nullptr;
	bool anyElementDead = false;
public:
	ArGraphicLayer() {}
	~ArGraphicLayer();

	void StartFrame(ArgonContext& context);

	void EndFrame(ArgonContext& context);

	void UpdateFocus(ArgonContext& context);

	ArGraphicElement* AddElement(ArGraphicElement* element);

	ArGraphicElement* HitTest(const ArVec2& pos) const;

	void SetFocus(ArGraphicElement* element);

	void LockFocus();

	void UnlockFocus();

	size_t GetElementCount() const { return elements.size(); }

	ArGraphicElement* GetElementByIndex(size_t index) const { if (index >= elements.size()) return nullptr; return elements[index]; }
};

// ----------------------------------------------------------- //
//  [ARGON] [ArgonGraphic] Basic Components declarations
// ----------------------------------------------------------- //

// [CLASS] [ArgonGraphicSystem] - Float animator component. This component is used to animate a float value.
class ArFloatAnimatorComp : public IArGraphicElementComponent
{
private:
	bool processing = false;

	std::function<float(float)> animationFunction = nullptr;

	ArDuration spentTime = 0ms;
	ArDuration length = 0ms;

	float startPoint = 0.f;
	float endPoint = 0.f;
public:
	float value = 0.f;
public:
	ArFloatAnimatorComp(
		float defaultValue = 0.f,
		ArDuration length = 200ms,
		std::function<float(float)> animationFunction = [](float t) { return t; }
	) : value(defaultValue), length(length), animationFunction(animationFunction)
	{
	}

	void OnUpdate(ArGraphicElement& owner, ArgonContext& context) override;

	void NewProcess(float start, float end, ArDuration length);

	void NewProcess(float end, ArDuration length) { NewProcess(value, end, length); }

	void SetAnimationFunction(std::function<float(float)> func) { animationFunction = func; }

	void SetLength(ArDuration length) { this->length = length; }

	bool IsProcessing() const { return processing; }

	ArFloatAnimatorComp& operator=(float value) { NewProcess(value, length); return *this; }

	operator float() const { return value; }

	operator const float& () const { return value; }
};

// [CLASS] [ArgonGraphicSystem] - Vec2 animator component. This component is used to animate a ArVec2 value.
class ArVec2AnimatorComp : public IArGraphicElementComponent
{
private:
	bool processing = false;
	std::function<ArVec2(float)> animationFunction = nullptr;
	ArDuration spentTime = 0ms;
	ArDuration length = 0ms;
	ArVec2 startPoint = {};
	ArVec2 endPoint = {};
public:
	ArVec2 value = {};
public:
	ArVec2AnimatorComp(
		ArVec2 defaultValue = {},
		ArDuration length = 200ms,
		std::function<ArVec2(float)> animationFunction = [](float t) { return ArVec2(t, t); }
	) : value(defaultValue), length(length), animationFunction(animationFunction)
	{
	}

	void OnUpdate(ArGraphicElement& owner, ArgonContext& context) override;

	void NewProcess(ArVec2 start, ArVec2 end, ArDuration length);

	void NewProcess(ArVec2 end, ArDuration length) { NewProcess(value, end, length); }

	void SetAnimationFunction(std::function<ArVec2(float)> func) { animationFunction = func; }

	void SetLength(ArDuration length) { this->length = length; }

	bool IsProcessing() const { return processing; }

	ArVec2AnimatorComp& operator=(ArVec2 value) { NewProcess(value, length); return *this; }

	operator ArVec2() const { return value; }

	operator const ArVec2& () const { return value; }

	operator ArVec2* () { return &value; }

	operator const ArVec2* () const { return &value; }
};

// [CLASS] [ArgonGraphicSystem] - Polygon collision component. This component is used to check if a point is inside a polygon.
class ArPolygonCollisionComp : public IArGraphicElementComponent
{
public:
	const std::vector<ArVec2> bounds = {};
	bool working = false;
public:
	bool hovering = false;

	ArPolygonCollisionComp(const std::vector<ArVec2>& bounds) : bounds(bounds), working(bounds.size() > 3) {}

	bool IsInBounds(ArVec2 pos);

	void OnUpdate(ArGraphicElement& owner, ArgonContext& context) override;
};

// [CLASS] [ArgonGraphicSystem] - Droppable component. This component allows the element to be dragged and dropped.
class ArDroppableComp : public IArGraphicElementComponent
{
private:
	bool isDragging = false;
	ArVec2 dragOffset = ArVec2(0.f, 0.f);
	ArGraphicElement* droppableElement = nullptr;
	ArVec2AnimatorComp* animator = nullptr;
public:
	ArDroppableComp(ArGraphicElement* droppableElement = nullptr, ArVec2AnimatorComp* animator = nullptr)
		: droppableElement(droppableElement), animator(animator) {
	}

	void Awake(ArGraphicElement& owner) override { if (!droppableElement) droppableElement = &owner; }

	void OnUpdate(ArGraphicElement& owner, ArgonContext& context) override;
};

// [CLASS] [ArgonGraphicSystem] - Flex item component. This component is used to set the flex properties of the element.
class ArFlexItemComp : public IArGraphicElementComponent
{
public:
	float flexGrow = 0.0f;
	float flexBasis = -1.0f;
	ArFlexLayoutAlignItems alignSelf = ArFlexLayoutAlignItems::None;
public:
	ArFlexItemComp() {}
	ArFlexItemComp(float grow, float basis = -1.0f, ArFlexLayoutAlignItems align = ArFlexLayoutAlignItems::None)
		: flexGrow(grow), flexBasis(basis), alignSelf(align) {
	}
	ArFlexItemComp(ArFlexLayoutAlignItems align)
		: alignSelf(align) {
	}
};

// [CLASS] [ArgonGraphicSystem] - Flex layout component. This component is used to set the layout rules for the element.
class ArFlexLayoutComp : public IArGraphicElementComponent
{
private:
	class FlexLine final
	{
	public:
		std::vector<ArGraphicElement*> items = {};
		float mainAxisSize = 0.f;
		float crossAxisSize = 0.f;
	};
private:
	const ArFlexLayoutDirection direction = ArFlexLayoutDirection::Vertical;
	const ArFlexLayoutJustifyContent justifyContent = ArFlexLayoutJustifyContent::FlexStart;
	const ArFlexLayoutAlignItems alignItems = ArFlexLayoutAlignItems::FlexStart;
	const ArFlexLayoutWrap wrap = ArFlexLayoutWrap::NoWrap;
	const ArFlexLayoutAlignContent alignContent = ArFlexLayoutAlignContent::FlexStart;
public:
	ArFlexLayoutComp(ArFlexLayoutDirection direction = ArFlexLayoutDirection::Vertical, ArFlexLayoutJustifyContent justifyContent = ArFlexLayoutJustifyContent::FlexStart, ArFlexLayoutAlignItems alignItems = ArFlexLayoutAlignItems::FlexStart, ArFlexLayoutWrap wrap = ArFlexLayoutWrap::NoWrap, ArFlexLayoutAlignContent alignContent = ArFlexLayoutAlignContent::FlexStart)
		: direction(direction), justifyContent(justifyContent), alignItems(alignItems), wrap(wrap), alignContent(alignContent) {
	}

	void Lay(ArGraphicElement& owner);

	void Awake(ArGraphicElement& owner) override { Lay(owner); }

	void OnUpdate(ArGraphicElement& owner, ArgonContext& context) override { Lay(owner); }
private:
	bool IsHorizontal() const { return direction == ArFlexLayoutDirection::Horizontal; }

	float GetMainSize(const ArVec2& size) const { return IsHorizontal() ? size.x : size.y; }

	float GetCrossSize(const ArVec2& size) const { return IsHorizontal() ? size.y : size.x; }

	float GetMainMargin(const ArGraphicBoxModel::Margin& m) const { return IsHorizontal() ? m.Left() + m.Right() : m.Top() + m.Bottom(); }

	float GetCrossMargin(const ArGraphicBoxModel::Margin& m) const { return IsHorizontal() ? m.Top() + m.Bottom() : m.Left() + m.Right(); }

	float& SelectMain(float& x, float& y) const { return IsHorizontal() ? x : y; }

	float& SelectCross(float& x, float& y) const { return IsHorizontal() ? y : x; }

	ArVec2 MakeVec(float main, float cross) const { return IsHorizontal() ? ArVec2(main, cross) : ArVec2(cross, main); }
};

// ----------------------------------------------------------- //
//  [ARGON] [ArgonGraphic] Internal elements declarations
// ----------------------------------------------------------- //

// [CLASS] [ArgonGraphicSystem] - Render list element for the graphic system. Only for the render list.
class ArGraphicPrimRenderListElement final : public ArGraphicElement
{
public:
	ArGraphicPrimRenderListElement() : ArGraphicElement(ArGraphicElementFlag::Enable | ArGraphicElementFlag::Visible) {}

	void Awake(ArgonContext& context) override;
};

// ----------------------------------------------------------- //
//  [ARGON] [ArgonGraphic] ArgonThemeManager declarations
// ----------------------------------------------------------- //

class ArGraphicStyleProperty final
{
public:
	using ValueType = std::variant<float, int, ArVec2>;

	ValueType value = {};

	ArGraphicStyleProperty(ValueType value) : value(value) {}

	operator float() const { if (!std::holds_alternative<float>(value)) throw std::bad_variant_access(); return std::get<float>(value); }
	operator int() const { if (!std::holds_alternative<int>(value)) throw std::bad_variant_access(); return std::get<int>(value); }
	operator ArVec2() const { if (!std::holds_alternative<ArVec2>(value)) throw std::bad_variant_access(); return std::get<ArVec2>(value); }
};

class IArGraphicStyleCollection
{
public:
	virtual std::unordered_map<ArStringView, ArGraphicStyleProperty> Properties() const = 0;
};

class IArGraphicColorPalette
{
public:
	virtual std::unordered_map<ArStringView, ArColor> Colors() const = 0;
};

class ArgonGraphicThemeManager final
{
public:
	std::unordered_map<ArStringView, ArGraphicStyleProperty> properties = {};
	std::unordered_map<ArStringView, ArColor> colors = {};

public:
	ArgonGraphicThemeManager() {}
	ArgonGraphicThemeManager(const ArgonGraphicThemeManager&) = delete;
	ArgonGraphicThemeManager& operator=(const ArgonGraphicThemeManager&) = delete;
	~ArgonGraphicThemeManager() {}

	bool SetDefualProperties(const IArGraphicStyleCollection& collection) { properties = collection.Properties(); return true; }

	bool SetDefualtColors(const IArGraphicColorPalette& palette) { colors = palette.Colors(); return true; }

	const ArGraphicStyleProperty& GetProperty(ArStringView name) const { auto it = properties.find(name); if (it != properties.end()) return it->second; throw std::out_of_range("Style property not found"); }

	const ArColor& GetColor(ArStringView name) const { auto it = colors.find(name); if (it != colors.end()) return it->second; throw std::out_of_range("Color not found"); }
};

// ----------------------------------------------------------- //
//  [ARGON] [ArgonGraphic] ArgonGraphicManager declarations
// ----------------------------------------------------------- //

// [CLASS] [ArgonGraphicSystem] - The main graphic manager class. This class is used to manage all the graphic elements and layers.
class ArgonGraphicManager final
{
private:
	ArGraphicLayer* backgroundLayer = new ArGraphicLayer;
	ArGraphicLayer* defaultLayer = new ArGraphicLayer;
	ArGraphicLayer* foregroundLayer = new ArGraphicLayer;
	std::vector<ArGraphicLayer*> layers = { backgroundLayer, defaultLayer, foregroundLayer };

	ArGraphicPrimRenderListElement* backgroundRenderListElement = new ArGraphicPrimRenderListElement;
	ArGraphicPrimRenderListElement* foregroundRenderListElement = new ArGraphicPrimRenderListElement;
public:
	ArGraphicElement* focusingElement = nullptr;
	ArGraphicElement* hoveringElement = nullptr;
public:
	ArgonGraphicManager() {}
	ArgonGraphicManager(const ArgonGraphicManager&) = delete;
	ArgonGraphicManager& operator=(const ArgonGraphicManager&) = delete;
	~ArgonGraphicManager();

	void Awake();

	void StartFrame(ArgonContext& context);

	void EndFrame(ArgonContext& context);

	ArGraphicLayer* AddLayer(ArLayerAdditionPriority priority = ArLayerAdditionPriority::AfterDefault);

	ArGraphicLayer* GetBackgroundLayer() const { return backgroundLayer; }

	ArGraphicLayer* GetDefaultLayer() const { return defaultLayer; }

	ArGraphicLayer* GetForegroundLayer() const { return foregroundLayer; }

	ArGraphicLayer* GetLayerByIndex(size_t index) const { if (index >= layers.size()) return nullptr; return layers[index]; }

	ArGraphicRenderList* GetBackgroundRenderList() const { return backgroundRenderListElement->renderList; }

	ArGraphicRenderList* GetForegroundRenderList() const { return foregroundRenderListElement->renderList; }

	const std::vector<ArGraphicLayer*>& GetLayers() const { return layers; }
private:
	void SetFocus(ArGraphicElement* element);

	void SetHover(ArGraphicElement* element);
};
