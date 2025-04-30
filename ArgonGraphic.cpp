#include "ArgonGui.h"
#include <stack>

ArGraphicElement::~ArGraphicElement()
{
	for (auto& [_, component] : componentMap) 
	{ 
		for (auto& c : component) delete c; 
	} 
	componentMap.clear(); 
	for (auto& child : children) 
		delete child; 
	children.clear();
}

void ArGraphicElement::Awaken(ArgonContext& context)
{
	root = GetRootElement();

	Awake(context);

	for (auto& child : children)
	{
		child->layer = layer;
		child->boundingBox.position = *boundingBox.position + *boundingBox.localPosition;
		child->Awaken(context);
	}
	for (auto& [typeIndex, components] : componentMap)
	{
		for (auto& component : components)
		{
			component->Awake(*this);
		}
	}
}

void ArGraphicElement::StartFrame(ArgonContext& context)
{
	if (visible)
		renderList = context.renderManager.AllocationRenderList<ArGraphicRenderList>(this);

	OnUpdate(context);
	if (focusing && context.graphicManager.focusingElement != this)
	{
		OnFocusLost(context);
		focusing = false;
	}

	for (auto& child : children)
	{
		child->boundingBox.position = *boundingBox.position + *boundingBox.localPosition;
		child->StartFrame(context);
	}
	for (auto& [_, components] : componentMap)
	{
		for (auto& component : components)
		{
			component->OnUpdate(*this, context);
		}
	}
}

void ArGraphicElement::EndFrame(ArgonContext& context)
{
	if (!visible)
		return;

	ArGraphicElement* parent = this->owner;
	ArRect finalRect = boundingBox.GetRect();
	while (parent)
	{
		finalRect = finalRect.Intersection(parent->boundingBox.GetRect());
		parent = parent->owner;
	}
	renderList->PushScissor(finalRect);
	OnRender(context);

	for (auto& child : children)
	{
		child->EndFrame(context);
	}
	for (auto& [_, components] : componentMap)
	{
		for (auto& component : components)
		{
			component->OnRender(*this);
		}
	}
}

ArGraphicElement* ArGraphicElement::GetRootElement()
{
	ArGraphicElement* root = this;
	while (root->owner)
	{
		root = root->owner;
	}
	return root;
}

int ArGraphicElement::FindMostDeepestChild() const
{
	int maxDepth = depth;
	for (const auto& child : children)
	{
		int childDepth = child->FindMostDeepestChild();
		if (childDepth > maxDepth)
		{
			maxDepth = childDepth;
		}
	}
	return maxDepth;
}

void ArGraphicElement::AddComponent(std::type_index typeIndex, IArGraphicComponent* component)
{
	componentMap[typeIndex].push_back(component);
}

IArGraphicComponent* ArGraphicElement::GetComponent(std::type_index typeIndex, size_t index)
{
	auto it = componentMap.find(typeIndex);
	if (it != componentMap.end())
	{
		if (index < 0 || index >= it->second.size())
			return nullptr;
		return it->second[index];
	}
	return nullptr;
}

bool ArGraphicElement::HasComponent(std::type_index typeIndex)
{
	auto it = componentMap.find(typeIndex);
	if (it != componentMap.end())
	{
		return true;
	}
	return false;
}

void ArGraphicElement::AddChild(ArGraphicElement* child)
{
	child->owner = this;
	child->depth = depth + 1;
	children.push_back(child);
}

ArGraphicElement* ArGraphicElement::GetChild(size_t index) const
{
	if (index >= children.size())
		return nullptr;
	auto it = std::next(children.begin(), index);
	return *it;
}

void ArGFloatAnimatorComp::OnUpdate(ArGraphicElement& owner, const ArgonContext& context)
{
	if (!processing)
		return;
	if (spentTime >= length)
	{
		processing = false;
		spentTime = 0ms;
		value = endPoint;
		return;
	}
	spentTime += context.GetDeltaTimeInArDuration();
	value = startPoint + (endPoint - startPoint) * animationFunction(std::chrono::duration<float>(spentTime).count() / std::chrono::duration<float>(length).count());
}

void ArGFloatAnimatorComp::StartNewProcess(float start, float end, ArDuration length)
{
	processing = true;
	spentTime = 0ms;
	startPoint = start;
	endPoint = end;
	this->length = length;
}

void ArGVec2AnimatorComp::OnUpdate(ArGraphicElement& owner, const ArgonContext& context)
{
	if (!processing)
		return;
	if (spentTime >= length)
	{
		processing = false;
		spentTime = 0ms;
		value = endPoint;
		return;
	}
	spentTime += context.GetDeltaTimeInArDuration();
	value = startPoint + (endPoint - startPoint) * animationFunction(std::chrono::duration<float>(spentTime).count() / std::chrono::duration<float>(length).count());
}

void ArGVec2AnimatorComp::StartNewProcess(ArVec2 start, ArVec2 end, ArDuration length)
{
	processing = true;
	spentTime = 0ms;
	startPoint = start;
	endPoint = end;
	this->length = length;
}

bool ArGPolygonCollisionComp::IsInBounds(ArVec2 pos)
{
	if (!working)
		return false;
	bool inside = false;
	size_t n = bounds.size();

	for (size_t i = 0, j = n - 1; i < n; j = i++)
	{
		bool intersect = ((bounds[i].y > pos.y) != (bounds[j].y > pos.y)) &&
			(pos.x < (bounds[j].x - bounds[i].x) * (pos.y - bounds[i].y) / (bounds[j].y - bounds[i].y) + bounds[i].x);

		if (intersect)
			inside = !inside;
	}

	return inside;
}

void ArGPolygonCollisionComp::OnUpdate(ArGraphicElement& owner, const ArgonContext& context)
{
	if (!working)
		return;
	const ArgonInputManager& inputManager = context.inputManager;
	ArVec2 currentMousePos = inputManager.GetMousePosition() - owner.boundingBox.GetActualPosition();
	hovering = IsInBounds(currentMousePos);
}

void ArGraphicPrimRenderListElement::Awake(const ArgonContext& context)
{
	interactive = false;
	boundingBox.size = context.inputManager.GetDisplayState().size;
}

void ArGraphicPrimRenderListElement::OnUpdate(const ArgonContext& context)
{
	boundingBox.size = context.inputManager.GetDisplayState().size;
}

void ArGraphicFocusSelectorElement::Awake(const ArgonContext& context)
{
	focusable = interactive = false;

	alphaAnimation = new ArGFloatAnimatorComp(0.f, [](float t) { return t; });
	positionAnimation = new ArGVec2AnimatorComp(ArVec2(0.f, 0.f), [](float t) { return ArVec2(t, t); });
	sizeAnimation = new ArGVec2AnimatorComp(ArVec2(0.f, 0.f), [](float t) { return ArVec2(t, t); });

	AddComponent(typeid(ArGFloatAnimatorComp), alphaAnimation);
	AddComponent(typeid(ArGVec2AnimatorComp), positionAnimation);
	AddComponent(typeid(ArGVec2AnimatorComp), sizeAnimation);

	boundingBox.size = context.inputManager.GetDisplayState().size;
}

void ArGraphicFocusSelectorElement::OnUpdate(const ArgonContext& context)
{
	visible = context.inputManager.GetLastInputDevice() == ArInputDevice::Gamepad;

	if (!visible)
		return;

	if (selected && context.graphicManager.focusingElement == nullptr)
	{
		alphaAnimation->StartNewProcess(255.f, 0.f, 500ms);
	}
	else if (!selected && context.graphicManager.focusingElement != nullptr)
	{
		alphaAnimation->StartNewProcess(0.f, 255.f, 500ms);
		boundingBox.position = context.graphicManager.focusingElement->boundingBox.GetActualPosition() - ArVec2(5.f, 5.f);
		boundingBox.size = *context.graphicManager.focusingElement->boundingBox.size + ArVec2(10.f, 10.f);
	}

	selected = context.graphicManager.focusingElement != nullptr;

	if (!selected)
		return;

	if (lastFocusBoundingBox != context.graphicManager.focusingElement->boundingBox)
	{
		ArVec2 size = *context.graphicManager.focusingElement->boundingBox.size + ArVec2(10.f, 10.f);
		ArVec2 position = context.graphicManager.focusingElement->boundingBox.GetActualPosition() - ArVec2(5.f, 5.f);

		if (lastFocusElement != context.graphicManager.focusingElement)
		{
			positionAnimation->StartNewProcess(*boundingBox.position, position, 200ms);
		}
		else
		{
			positionAnimation->value = position;
		}

		sizeAnimation->StartNewProcess(*boundingBox.size, size, 300ms);
	}

	lastFocusElement = context.graphicManager.focusingElement;
	lastFocusBoundingBox = context.graphicManager.focusingElement->boundingBox;

	boundingBox.position = positionAnimation->value;
	boundingBox.size = sizeAnimation->value;
}

void ArGraphicFocusSelectorElement::OnRender(const ArgonContext& context)
{
	renderList->AddRect(boundingBox.GetRect(), AR_COL32(255.f, 255.f, 255.f, alphaAnimation->value), 2.f);
}

ArGraphicLayer::~ArGraphicLayer()
{
	while (!unwakedElements.empty())
	{
		ArGraphicElement* element = unwakedElements.back();
		unwakedElements.pop();
		delete element;
	}

	for (ArGraphicElement* element : elements)
	{
		delete element;
	}
	elements.clear();
}

void ArGraphicLayer::StartFrame(ArgonContext& context)
{
	while (!unwakedElements.empty())
	{
		ArGraphicElement*& element = unwakedElements.front();
		unwakedElements.pop();
		element->Awaken(context);
		elements.push_back(element);
	}

	for (ArGraphicElement* element : elements)
	{
		element->StartFrame(context);
	}
}

void ArGraphicLayer::EndFrame(ArgonContext& context)
{
	for (auto it = elements.begin(); it != elements.end(); )
	{
		ArGraphicElement* element = *it;
		element->EndFrame(context);

		if (element->dead)
		{
			it = elements.erase(it);
			delete element;
		}
		else
		{
			++it;
		}
	}
}

void ArGraphicLayer::UpdateFocus(ArgonContext& context)
{
	ArGraphicElement* currentHoveringElement = HitTest(context.inputManager.GetMousePosition());
	if (!lockFocus)
	{
		if (currentHoveringElement != hoveringElement)
		{
			if (hoveringElement) hoveringElement->hovered = false;
			if (currentHoveringElement) currentHoveringElement->hovered = true;
			hoveringElement = currentHoveringElement;
		}
		if (context.inputManager.IsMouseButtonDown(ArMouseButton::Left))
		{
			if (hoveringElement)
			{
				hoveringElement->focusing = true;
				SetFocus(hoveringElement);
			}
			else
			{
				SetFocus(nullptr);
			}
		}
	}
}

ArGraphicElement* ArGraphicLayer::AddElement(ArGraphicElement* element)
{
	element->layer = this;
	unwakedElements.push(element);
	return element;
}

ArGraphicElement* ArGraphicLayer::HitTest(const ArVec2& pos) const
{
	ArGraphicElement* topMostElement = nullptr;
	int32_t topMostDepth = INT32_MIN;
	std::stack<ArGraphicElement*> stack = {};
	std::for_each(elements.begin(), elements.end(), [&stack](ArGraphicElement* element) { stack.push(element); });

	while (!stack.empty())
	{
		ArGraphicElement* element = stack.top();
		stack.pop();

		if (!element->interactive || element->dead || !element->visible)
			continue;

		if (!element->HitTest(pos))
			continue;

		if (element->focusable && element->depth > topMostDepth)
		{
			// Check if the element's index of root is greater than the topMostElement's index
			if (topMostElement)
			{
				auto it = std::find(elements.begin(), elements.end(), element->root);
				auto it2 = std::find(elements.begin(), elements.end(), topMostElement->root);
				if (it < it2)
					continue;
			}
			topMostDepth = element->depth;
			topMostElement = element;
		}
		for (const auto& child : element->children)
		{
			stack.push(child);
		}
	}

	return topMostElement;
}

void ArGraphicLayer::SetFocus(ArGraphicElement* element)
{
	if (element == focusingElement)
		return;
	if (focusingElement) focusingElement->focusing = false;
	if (element)
	{
		element->focusing = true;
		auto it = std::find(elements.begin(), elements.end(), element->root);
		std::rotate(it, it + 1, elements.end());
	}
	focusingElement = element;
}

void ArGraphicLayer::LockFocus()
{
	lockFocus = true;
}

void ArGraphicLayer::UnlockFocus()
{
	lockFocus = false;
}

ArgonGraphicManager::~ArgonGraphicManager()
{
	for (ArGraphicLayer* layer : layers)
	{
		delete layer;
	}
	layers.clear();
}

void ArgonGraphicManager::Awake()
{
	backgroundLayer->AddElement(backgroundRenderListElement);
	foregroundLayer->AddElement(foregroundRenderListElement);
	foregroundLayer->AddElement(focusSelectorElement);
}

void ArgonGraphicManager::StartFrame(ArgonContext& context)
{
	bool foundFocus = false;
	bool foundHover = false;
	for (auto it = layers.rbegin(); it != layers.rend(); ++it)
	{
		ArGraphicLayer* layer = *it;
		layer->UpdateFocus(context);
		if (!layer->focusingElement && !layer->hoveringElement)
			continue;
		if (layer->focusingElement)
		{
			foundFocus = true;
			focusingElement = layer->focusingElement;
		}
		if (layer->hoveringElement)
		{
			foundHover = true;
			hoveringElement = layer->hoveringElement;
		}
		break;
	}
	if (!foundFocus)
	{
		focusingElement = nullptr;
	}
	if (!foundHover)
	{
		hoveringElement = nullptr;
	}

	for (ArGraphicLayer* layer : layers)
	{
		layer->StartFrame(context);
	}
}

void ArgonGraphicManager::EndFrame(ArgonContext& context)
{
	for (ArGraphicLayer* layer : layers)
	{
		layer->EndFrame(context);
	}
}

ArGraphicLayer* ArgonGraphicManager::AddLayer(ArLayerAdditionPriority priority)
{
	ArGraphicLayer* newLayer = new ArGraphicLayer;
	// 1.find the position
	ArGraphicLayer* baseLayer = nullptr;
	int offset = 0;
	switch (priority)
	{
	case ArLayerAdditionPriority::BeforeDefault:
		baseLayer = defaultLayer;
		offset = 0;
		break;
	case ArLayerAdditionPriority::AfterDefault:
		baseLayer = defaultLayer;
		offset = 1;
		break;
	case ArLayerAdditionPriority::AfterBackground:
		baseLayer = backgroundLayer;
		offset = 1;
		break;
	case ArLayerAdditionPriority::BeforeForeground:
		baseLayer = foregroundLayer;
		offset = 0;
		break;
	case ArLayerAdditionPriority::None:
		return defaultLayer;
	}
	const auto& baseIt = std::find(layers.begin(), layers.end(), baseLayer);
	// 2. push the new layer to the position
	layers.insert(baseIt + offset, newLayer);
	return newLayer;
}
