#include "ArgonCore.h"
#include <stack>

void ArGraphicElement::Awaken(ArgonContext& context)
{
	root = GetRootElement();

	Awake(context);

	for (auto& child : children)
	{
		child->layer = layer;
		child->boundingBox.position = boundingBox.position + boundingBox.localPosition;
		child->Awaken(context);
	}
	for (auto& [typeIndex, component] : componentMap)
	{
		component->Awake(*this);
	}
}

void ArGraphicElement::StartFrame(ArgonContext& context)
{
	if (!visible)
		return;
	renderList = context.renderManager.AllocationRenderList<ArGraphicRenderList>(this);

	OnUpdate(context);
	if (focusing && context.graphicManager.focusingElement != this)
	{
		OnFocusLost(context);
		focusing = false;
	}

	for (auto& child : children)
	{
		child->boundingBox.position = boundingBox.position + boundingBox.localPosition;
		child->StartFrame(context);
	}
	for (auto& [_, component] : componentMap)
	{
		component->OnUpdate(*this, context);
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
	for (auto& [_, component] : componentMap)
	{
		component->OnRender(*this);
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
	componentMap[typeIndex] = component;
}

IArGraphicComponent* ArGraphicElement::GetComponent(std::type_index typeIndex)
{
	auto it = componentMap.find(typeIndex);
	if (it != componentMap.end())
	{
		return it->second;
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

void ArGraphicPrimRenderListElement::Awake(const ArgonContext& context)
{
	interactive = false;
	boundingBox.size = context.inputManager.GetDisplayState().size;
}

void ArGraphicPrimRenderListElement::OnUpdate(const ArgonContext& context)
{
	boundingBox.size = context.inputManager.GetDisplayState().size;
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

		if (!element->interactive || element->dead)
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
}

void ArgonGraphicManager::StartFrame(ArgonContext& context)
{
	for (auto it = layers.rbegin(); it != layers.rend(); ++it)
	{
		ArGraphicLayer* layer = *it;
		layer->UpdateFocus(context);
		if (!layer->focusingElement && !layer->hoveringElement)
			continue;
		if (layer->focusingElement)
			focusingElement = layer->focusingElement;
		if (layer->hoveringElement)
			hoveringElement = layer->hoveringElement;
		break;
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

ArGraphicLayer* ArgonGraphicManager::AddLayer()
{
	// push the new layer to the position between the default and foreground layer
	ArGraphicLayer* newLayer = new ArGraphicLayer;
	layers.reserve(layers.size() + 1);
	layers.resize(layers.size() - 1);
	layers.push_back(newLayer);
	layers.push_back(foregroundLayer);
	return newLayer;
}
