#pragma once
#include <typeindex>

class ArGraphicRenderList;
class ArGraphicElement;
class ArGraphicLayer;

class IArGraphicComponent
{
public:
	virtual void Awake(ArGraphicElement& owner) {};
	virtual void OnUpdate(ArGraphicElement& owner, const ArgonContext& context) = 0;
	virtual void OnRender(ArGraphicElement& owner) {};
};

class ArGraphicRenderList final : public ArRenderList
{
public:
	ArGraphicElement* owner = nullptr;
public:
	ArGraphicRenderList(ArRenderListSharedData* sharedData, ArGraphicElement* owner) : ArRenderList(sharedData), owner(owner) {}
};

class ArGraphicBoundingBox final
{
public:
	ArVec2 position = ArVec2();			// Position of owner element
	ArVec2 localPosition = ArVec2();	// Offset to origin of the owner element
	ArVec2 size = ArVec2();
public:
	ArRect GetRect() const { return ArRect(position + localPosition, position + localPosition + size); }

	ArVec2 GetActualPosition() const { return position + localPosition; }
};

class ArGraphicElement
{
public:
	ArGraphicLayer* layer = nullptr;
	ArGraphicElement* owner = nullptr;
	ArGraphicElement* root = nullptr;

	ArGraphicBoundingBox boundingBox = ArGraphicBoundingBox();
	ArGraphicRenderList* renderList = nullptr;
	std::list<ArGraphicElement*> children = {};
	std::unordered_map<std::type_index, IArGraphicComponent*> componentMap = {};

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
	virtual ~ArGraphicElement() { for (auto& [_, component] : componentMap) delete component; for (auto& child : children) delete child; }


	void Awaken(ArgonContext& context);

	void StartFrame(ArgonContext& context);

	void EndFrame(ArgonContext& context);


	ArGraphicElement* GetRootElement();

	int FindMostDeepestChild() const;

	void AddComponent(std::type_index typeIndex, IArGraphicComponent* component);

	IArGraphicComponent* GetComponent(std::type_index typeIndex);

	bool HasComponent(std::type_index typeIndex);

	void AddChild(ArGraphicElement* child);

	ArGraphicElement* GetChild(size_t index) const;


	virtual void Awake(const ArgonContext& context) {}

	virtual void OnUpdate(const ArgonContext& context) {}

	virtual void OnRender(const ArgonContext& context) {}

	virtual void OnFocusLost(const ArgonContext& context) {}

	virtual bool HitTest(const ArVec2& pos) const { return boundingBox.GetRect().Contains(pos); }
};

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
};

class ArGraphicPrimRenderListElement final : public ArGraphicElement 
{
public:
	void Awake(const ArgonContext& context) override;

	void OnUpdate(const ArgonContext& context) override;
};

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


	ArGraphicLayer* AddLayer();

	ArGraphicLayer* GetBackgroundLayer() const { return backgroundLayer; }

	ArGraphicLayer* GetDefaultLayer() const { return defaultLayer; }

	ArGraphicLayer* GetForegroundLayer() const { return foregroundLayer; }

	ArGraphicLayer* GetLayerByIndex(size_t index) const { if (index >= layers.size()) return nullptr; return layers[index]; }

	ArGraphicRenderList* GetBackgroundRenderList() const { return backgroundRenderListElement->renderList; }

	ArGraphicRenderList* GetForegroundRenderList() const { return foregroundRenderListElement->renderList; }
};
