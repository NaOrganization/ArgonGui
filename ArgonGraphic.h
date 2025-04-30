#pragma once
#include <typeindex>

// ----------------------------------------------------------- //
//  [ARGON] [ArgonGraphic] Enum class declarations
// ----------------------------------------------------------- //

// [ENUM] [ArgonGraphicSystem] - To describe the priority of the layer addition.
enum class ArLayerAdditionPriority : uint32_t
{
	BeforeDefault,
	AfterDefault,
	AfterBackground,
	BeforeForeground,
	None
};

// ----------------------------------------------------------- //
//  [ARGON] [ArgonGraphic] Interface declarations
// ----------------------------------------------------------- //

// [INTERFACE] [ArgonGraphicSystem] - Graphic component interface. This interface is used to create custom components for the graphic system.
class IArGraphicComponent
{
public:
	virtual void Awake(ArGraphicElement& owner) {};
	virtual void OnUpdate(ArGraphicElement& owner, const ArgonContext& context) = 0;
	virtual void OnRender(ArGraphicElement& owner) {};
};

// ---------------------------------------------------------- //
// !. Element declarations
// ---------------------------------------------------------- //

// [CLASS] [ArgonGraphicSystem] - Render list with owner element.
class ArGraphicRenderList final : public ArRenderList
{
public:
	ArGraphicElement* owner = nullptr;
public:
	ArGraphicRenderList(ArRenderListSharedData* sharedData, ArGraphicElement* owner) : ArRenderList(sharedData), owner(owner) {}
};

// [STRUCT] [ArgonGraphicSystem] - Bounding box of the element.
class ArGraphicBoundingBox final
{
public:
	ArValueSubject<ArVec2> position = ArVec2();			// Position of owner element
	ArValueSubject<ArVec2> localPosition = ArVec2();	// Offset to origin of the owner element
	ArValueSubject<ArVec2> size = ArVec2();
public:
	ArRect GetRect() const { return ArRect(*position + *localPosition, *position + *localPosition + *size); }

	ArVec2 GetActualPosition() const { return *position + *localPosition; }

	bool operator==(const ArGraphicBoundingBox& other) const
	{
		return *position == *other.position && *localPosition == *other.localPosition && *size == *other.size;
	}
	bool operator!=(const ArGraphicBoundingBox& other) const
	{
		return !(*this == other);
	}
};

// [CLASS] [ArgonGraphicSystem] - Basic element of the graphic system.
class ArGraphicElement
{
public:
	ArGraphicLayer* layer = nullptr;
	ArGraphicElement* owner = nullptr;
	ArGraphicElement* root = nullptr;

	ArGraphicBoundingBox boundingBox = ArGraphicBoundingBox();
	ArGraphicRenderList* renderList = nullptr;
	std::list<ArGraphicElement*> children = {};
	std::unordered_map<std::type_index, std::vector<IArGraphicComponent*>> componentMap = {};

	int depth = 0;
	bool focusing = false;
	bool hovered = false;

	bool dead = false;
	bool visible = true;
	bool interactive = true;
	bool focusable = true;
public:
	ArGraphicElement(ArGraphicElement* owner = nullptr) : owner(owner) {}
	ArGraphicElement(const ArGraphicElement&) = delete;
	ArGraphicElement& operator=(const ArGraphicElement&) = delete;
	virtual ~ArGraphicElement();


	void Awaken(ArgonContext& context);

	void StartFrame(ArgonContext& context);

	void EndFrame(ArgonContext& context);


	ArGraphicElement* GetRootElement();

	int FindMostDeepestChild() const;

	void AddComponent(std::type_index typeIndex, IArGraphicComponent* component);

	IArGraphicComponent* GetComponent(std::type_index typeIndex, size_t index = 0);

	bool HasComponent(std::type_index typeIndex);

	void AddChild(ArGraphicElement* child);

	ArGraphicElement* GetChild(size_t index) const;


	virtual void Awake(const ArgonContext& context) {}

	virtual void OnUpdate(const ArgonContext& context) {}

	virtual void OnRender(const ArgonContext& context) {}

	virtual void OnFocusLost(const ArgonContext& context) {}

	virtual bool HitTest(const ArVec2& pos) const { return boundingBox.GetRect().Contains(pos); }
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

// ---------------------------------------------------------- //
// !. Basic Components declarations
// ---------------------------------------------------------- //

// [CLASS] [ArgonGraphicSystem] - Float animator component. This component is used to animate a float value.
class ArGFloatAnimatorComp : public IArGraphicComponent
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
	ArGFloatAnimatorComp(
		float defaultValue = 0.f,
		std::function<float(float)> animationFunction = [](float t) { return t; }
	) : animationFunction(animationFunction),
		value(defaultValue)
	{
	}

	void OnUpdate(ArGraphicElement& owner, const ArgonContext& context) override;

	void StartNewProcess(float start, float end, ArDuration length);

	void SetAnimationFunction(std::function<float(float)> func) { animationFunction = func; }
};

// [CLASS] [ArgonGraphicSystem] - Vec2 animator component. This component is used to animate a ArVec2 value.
class ArGVec2AnimatorComp : public IArGraphicComponent
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
	ArGVec2AnimatorComp(
		ArVec2 defaultValue = ArVec2(0.f, 0.f),
		std::function<ArVec2(float)> animationFunction = [](float t) { return ArVec2(t, t); }
	) : animationFunction(animationFunction),
		value(defaultValue)
	{
	}

	void OnUpdate(ArGraphicElement& owner, const ArgonContext& context) override;

	void StartNewProcess(ArVec2 start, ArVec2 end, ArDuration length);

	void SetAnimationFunction(std::function<ArVec2(float)> func) { animationFunction = func; }
};

// [CLASS] [ArgonGraphicSystem] - Polygon collision component. This component is used to check if a point is inside a polygon.
class ArGPolygonCollisionComp : public IArGraphicComponent
{
public:
	const std::vector<ArVec2> bounds = {};
	bool working = false;
public:
	bool hovering = false;

	ArGPolygonCollisionComp(const std::vector<ArVec2>& bounds) : bounds(bounds), working(bounds.size() > 3) {}

	bool IsInBounds(ArVec2 pos);

	void OnUpdate(ArGraphicElement& owner, const ArgonContext& context) override;
};

// ------------------------------------------------------------- //
// !. Graphic System Internal Elements declarations
// ------------------------------------------------------------- //

// [CLASS] [ArgonGraphicSystem] - Render list element for the graphic system. Only for the render list.
class ArGraphicPrimRenderListElement final : public ArGraphicElement 
{
public:
	void Awake(const ArgonContext& context) override;

	void OnUpdate(const ArgonContext& context) override;
};

// [CLASS] [ArgonGraphicSystem] - Focus selector element for the graphic system. This element is used to show the focus selector.
class ArGraphicFocusSelectorElement : public ArGraphicElement
{
public:
	bool selected = false;
	ArGVec2AnimatorComp* positionAnimation = nullptr;
	ArGVec2AnimatorComp* sizeAnimation = nullptr;
	ArGFloatAnimatorComp* alphaAnimation = nullptr;
	ArGraphicBoundingBox lastFocusBoundingBox = {};
	ArGraphicElement* lastFocusElement = nullptr;

	void Awake(const ArgonContext& context) override;

	void OnUpdate(const ArgonContext& context) override;

	void OnRender(const ArgonContext& context) override;
};

// ----------------------------------------------------------- //
// ! ArgonThemeManager declarations
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
	virtual std::unordered_map<ArStringView, ArGraphicStyleProperty>& GetProperties() const = 0;
};

class IArGraphicColorPalette
{
public:
	virtual std::unordered_map<ArStringView, ArColor>& GetColors() const = 0;
};

class ArgonGraphicThemeManager final
{
public:
	std::unordered_map<ArStringView, ArGraphicStyleProperty> properties = {};
	std::unordered_map<ArStringView, ArColor> colors = {};

	ArgonGraphicThemeManager() {}
	ArgonGraphicThemeManager(const ArgonGraphicThemeManager&) = delete;
	ArgonGraphicThemeManager& operator=(const ArgonGraphicThemeManager&) = delete;
	~ArgonGraphicThemeManager() {}

	bool ImportStyleCollection(const IArGraphicStyleCollection& collection) { properties = collection.GetProperties(); return true; }

	bool ImportColorPalette(const IArGraphicColorPalette& palette) { colors = palette.GetColors(); return true; }

	const ArGraphicStyleProperty& GetStyleProperty(ArStringView name) const
	{
		auto it = properties.find(name);
		if (it != properties.end())
			return it->second;
		throw std::out_of_range("Style property not found");
	}

	const ArColor& GetColor(ArStringView name) const
	{
		auto it = colors.find(name);
		if (it != colors.end())
			return it->second;
		throw std::out_of_range("Color not found");
	}
};

// ---------------------------------------------------------- //
// !. ArgonGraphicManager declarations
// ---------------------------------------------------------- //

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
	ArGraphicFocusSelectorElement* focusSelectorElement = new ArGraphicFocusSelectorElement;
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
};
