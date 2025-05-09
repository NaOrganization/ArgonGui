#include "ArgonGui.h"
#include <stack>
#include "ArgonGraphic.h"

ArGraphicElement::~ArGraphicElement()
{
	for (auto& [_, component] : componentMap)
	{
		for (auto& c : component) delete c;
	}
	componentMap.clear();

	while (!unwakedChildren.empty())
	{
		ArGraphicElement* child = unwakedChildren.front();
		unwakedChildren.pop_front();
		delete child;
	}

	for (auto& child : children)
		delete child;
	children.clear();
}

void ArGraphicElement::Awaken(ArgonContext& context)
{
	root = GetRootElement();

	Awake(context);

	while (!unwakedChildren.empty())
	{
		ArGraphicElement* child = unwakedChildren.front();
		unwakedChildren.pop_front();
		children.push_back(child);
	}

	for (auto& child : children)
	{
		child->layer = layer;
		child->borderBox.position = borderBox.position + borderBox.localPosition;
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

void ArGraphicElement::FrameUpdate(ArgonContext& context)
{
	if (!ArHasFlag(flags, ArGraphicElementFlag::Enable))
		return;

	if (ArHasFlag(flags, ArGraphicElementFlag::Visible))
	{
		renderList = context.renderManager.AllocationRenderList<ArGraphicRenderList>(this);
		ArGraphicElement* parent = this->owner;
		if (!borderBox.overflow && ArLackFlag(flags, ArGraphicElementFlag::Debug))
		{
			ArRect finalRect = borderBox.GetBorderRect();
			while (parent)
			{
				finalRect = finalRect.Intersection(parent->borderBox.GetContentRect());
				parent = parent->owner;
			}
			renderList->PushScissor(finalRect);
		}
		else
		{
			renderList->PushScissor(ArRect({}, context.inputManager.GetDisplayState().size));
		}

		OnRender(context);

		for (auto& [_, components] : componentMap)
		{
			for (auto& component : components)
			{
				component->OnRender(*this);
			}
		}

		if (ArHasFlag(flags, ArGraphicElementFlag::Debug))
		{
			renderList->AddRect(borderBox.GetMarginRect(), AR_COL32(255.f, 0.f, 0.f, 255.f));
			renderList->AddRect(borderBox.GetPaddingRect(), AR_COL32(0.f, 255.f, 0.f, 255.f));
			renderList->AddRect(borderBox.GetContentRect(), AR_COL32(0.f, 0.f, 255.f, 255.f));
		}
	}

	while (!unwakedChildren.empty())
	{
		ArGraphicElement* child = unwakedChildren.front();
		unwakedChildren.pop_front();
		children.push_back(child);
		child->layer = layer;
		child->borderBox.position = borderBox.position + borderBox.localPosition;
		child->Awaken(context);
	}

	for (auto& child : children)
	{
		child->borderBox.position = borderBox.position + borderBox.localPosition;
		child->FrameUpdate(context);
	}

	if (ArHasFlag(beingState, ArGraphicElementBeingState::Focus))
	{
		OnFocus(context);
	}
	else if (ArHasFlag(beingState, ArGraphicElementBeingState::Unfocus))
	{
		OnFocusLost(context);
	}

	if (ArHasFlag(beingState, ArGraphicElementBeingState::Hover))
	{
		OnHover(context);
	}
	else if (ArHasFlag(beingState, ArGraphicElementBeingState::Unhover))
	{
		OnUnhover(context);
	}

	OnUpdate(context);

	for (auto& [_, components] : componentMap)
	{
		for (auto& component : components)
		{
			component->OnUpdate(*this, context);
			if (ArHasAnyFlags(beingState, ArGraphicElementBeingState::ChildAdded, ArGraphicElementBeingState::ChildRemoved))
				component->OnChildrenListChanged(*this, context);
		}
	}

	ArIfRemoveFlags(beingState, ArGraphicElementBeingState::All);
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

void ArGraphicElement::AddChild(ArGraphicElement* child)
{
	child->owner = this;
	child->depth = depth + 1;
	unwakedChildren.push_back(child);
}

ArGraphicElement* ArGraphicElement::GetChild(size_t index) const
{
	if (index >= children.size())
		return nullptr;
	auto it = std::next(children.begin(), index);
	return *it;
}

std::vector<ArGraphicElement*> ArGraphicElement::GetAvailableChildren() const
{
	std::vector<ArGraphicElement*> availableChildren;
	availableChildren.reserve(children.size());
	for (auto& child : children)
	{
		if (ArLackFlag(child->flags, ArGraphicElementFlag::Enable)
			|| ArHasFlag(child->flags, ArGraphicElementFlag::Dead))
			continue;
		availableChildren.push_back(child);
	}
	return availableChildren;
}

void ArGraphicElement::Kill()
{
	flags |= ArGraphicElementFlag::Dead;
	layer->anyElementDead = true;
}

void ArFloatAnimatorComp::OnUpdate(ArGraphicElement& owner, ArgonContext& context)
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

void ArFloatAnimatorComp::NewProcess(float start, float end, ArDuration length)
{
	processing = true;
	spentTime = 0ms;
	startPoint = start;
	endPoint = end;
	this->length = length;
}

void ArVec2AnimatorComp::OnUpdate(ArGraphicElement& owner, ArgonContext& context)
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

void ArVec2AnimatorComp::NewProcess(ArVec2 start, ArVec2 end, ArDuration length)
{
	processing = true;
	spentTime = 0ms;
	startPoint = start;
	endPoint = end;
	this->length = length;
}

bool ArPolygonCollisionComp::IsInBounds(ArVec2 pos)
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

void ArPolygonCollisionComp::OnUpdate(ArGraphicElement& owner, ArgonContext& context)
{
	if (!working)
		return;
	const ArgonInputManager& inputManager = context.inputManager;
	ArVec2 currentMousePos = inputManager.GetMousePosition() - owner.borderBox.GetContentPosition();
	hovering = IsInBounds(currentMousePos);
}

void ArDroppableComp::OnUpdate(ArGraphicElement& owner, ArgonContext& context)
{
	if (!owner.focusing || !owner.hovering)
		return;
	const ArgonInputManager& inputManager = context.inputManager;
	ArVec2 currentMousePos = inputManager.GetMousePosition();
	if (inputManager.IsMouseButtonDown(ArMouseButton::Left))
	{
		isDragging = true;
		dragOffset = currentMousePos - droppableElement->borderBox.GetContentPosition();
		owner.layer->LockFocus();
	}
	if (!isDragging)
	{
		if (animator)
			animator->value = droppableElement->borderBox.GetContentPosition();
		return;
	}
	if (!inputManager.IsMouseButtonHeld(ArMouseButton::Left))
	{
		isDragging = false;
		dragOffset = ArVec2(0.f, 0.f);
		owner.layer->UnlockFocus();
		return;
	}
	if (animator)
	{
		*animator = currentMousePos - dragOffset;
		droppableElement->borderBox.position = animator->value;
	}
	else
	{
		droppableElement->borderBox.position = currentMousePos - dragOffset;
	}
}

void ArFlexLayoutComp::Lay(ArGraphicElement& owner)
{
	if (owner.GetAvailableChildren().size() <= 0)
		return;

	const ArVec2 availableSize = owner.borderBox.GetContentSize();
	const float containerMainSize = GetMainSize(availableSize);

	std::vector<ArGraphicElement*> children = owner.GetAvailableChildren();
	if (children.empty())
		return;

	for (ArGraphicElement* child : children)
	{
		if (!child->HasComponent<ArFlexItemComp>())
			child->AddComponent(new ArFlexItemComp());
		auto* flexItem = child->GetComponent<ArFlexItemComp>();
		if (flexItem->flexBasis < 0.f)
		{
			flexItem->flexBasis = GetMainSize(child->borderBox.GetContentSize());
		}
	}

	std::vector<FlexLine> lines;
	FlexLine currentLine;

	float lineMainSize = 0.f;
	float lineCrossSize = 0.f;

	for (auto* child : children)
	{
		auto* flexItem = child->GetComponent<ArFlexItemComp>();
		if (!flexItem)
			continue;
		ArVec2 childSize = child->borderBox.GetContentSize();
		auto& margin = child->borderBox.margin;

		float childMainSize = GetMainSize(childSize) + GetMainMargin(margin);
		float childCrossSize = GetCrossSize(childSize) + GetCrossMargin(margin);

		// Handle wrapping
		if (wrap != ArFlexLayoutWrap::NoWrap && !currentLine.items.empty()
			&& lineMainSize + childMainSize > containerMainSize)
		{
			currentLine.mainAxisSize = lineMainSize;
			currentLine.crossAxisSize = lineCrossSize;
			lines.push_back(std::move(currentLine));
			currentLine = FlexLine();

			lineMainSize = 0.f;
			lineCrossSize = 0.f;
		}

		currentLine.items.push_back(child);
		lineMainSize += flexItem->flexBasis;
		lineCrossSize = std::max(lineCrossSize, childCrossSize);
	}

	// Push the last line
	if (!currentLine.items.empty())
	{
		currentLine.mainAxisSize = lineMainSize;
		currentLine.crossAxisSize = lineCrossSize;
		lines.push_back(std::move(currentLine));
	}

	// Cross-axis offset for alignContent
	float totalCrossSize = 0.f;
	for (const auto& line : lines) totalCrossSize += line.crossAxisSize;

	float crossOffset = 0.f;
	float crossSpacing = 0.f;

	switch (alignContent)
	{
	case ArFlexLayoutAlignContent::Center:
		crossOffset = (GetCrossSize(availableSize) - totalCrossSize) * 0.5f;
		break;
	case ArFlexLayoutAlignContent::Stretch:
		if (!lines.empty())
		{
			float totalCross = 0.f;
			for (auto& line : lines) totalCross += line.crossAxisSize;
			float remain = GetCrossSize(availableSize) - totalCross;
			if (remain > 0.f) {
				float extra = remain / lines.size();
				for (auto& line : lines) line.crossAxisSize += extra;
			}
		}
		break;
	case ArFlexLayoutAlignContent::FlexEnd:
		crossOffset = GetCrossSize(availableSize) - totalCrossSize;
		break;
	case ArFlexLayoutAlignContent::SpaceBetween:
		if (lines.size() > 1)
			crossSpacing = (GetCrossSize(availableSize) - totalCrossSize) / (lines.size() - 1);
		break;
	case ArFlexLayoutAlignContent::SpaceAround:
		if (lines.size() > 0)
			crossSpacing = (GetCrossSize(availableSize) - totalCrossSize) / lines.size();
		crossOffset = crossSpacing * 0.5f;
		break;
	case ArFlexLayoutAlignContent::SpaceEvenly:
		if (lines.size() > 0)
			crossSpacing = (GetCrossSize(availableSize) - totalCrossSize) / (lines.size() + 1);
		crossOffset = crossSpacing;
		break;
	default:
		break;
	}

	// Layout each line
	for (const auto& line : lines)
	{
		// Calculate main-axis justification offset
		float mainOffset = 0.f;
		float extraSpace = containerMainSize - line.mainAxisSize;
		float gap = 0.f;
		float totalFlexGrow = 0.f;

		switch (justifyContent)
		{
		case ArFlexLayoutJustifyContent::Center:
			mainOffset = extraSpace * 0.5f;
			break;
		case ArFlexLayoutJustifyContent::FlexEnd:
			mainOffset = extraSpace;
			break;
		case ArFlexLayoutJustifyContent::SpaceBetween:
			if (line.items.size() > 1)
				gap = extraSpace / (line.items.size() - 1);
			break;
		case ArFlexLayoutJustifyContent::SpaceAround:
			gap = extraSpace / line.items.size();
			mainOffset = gap * 0.5f;
			break;
		case ArFlexLayoutJustifyContent::SpaceEvenly:
			gap = extraSpace / (line.items.size() + 1);
			mainOffset = gap;
			break;
		default:
			break;
		}

		// Calculate flex-grow
		for (auto* child : line.items)
		{
			auto* flexItem = child->GetComponent<ArFlexItemComp>();
			if (!flexItem)
				continue;
			totalFlexGrow += flexItem->flexGrow;
		}

		// Layout children in line
		float currentMain = mainOffset;
		for (auto* child : line.items)
		{
			if (!child) continue;
			auto* flexItem = child->GetComponent<ArFlexItemComp>();
			if (!flexItem)
				continue;

			ArVec2 childSize = child->borderBox.GetContentSize();
			auto& margin = child->borderBox.margin;

			float childMainSize = GetMainSize(childSize);
			float childCrossSize = GetCrossSize(childSize);

			// Cross-alignment: alignItems / alignSelf
			ArFlexLayoutAlignItems align = alignItems;
			if (flexItem->alignSelf != ArFlexLayoutAlignItems::None)
				align = flexItem->alignSelf;

			float crossPos = crossOffset;
			switch (align)
			{
			case ArFlexLayoutAlignItems::Center:
				crossPos += (line.crossAxisSize - childCrossSize) * 0.5f;
				break;
			case ArFlexLayoutAlignItems::FlexEnd:
				crossPos += (line.crossAxisSize - childCrossSize);
				break;
			case ArFlexLayoutAlignItems::Stretch:
			{
				if (IsHorizontal()) 
				{
					float newHeight = line.crossAxisSize - margin.Top() - margin.Bottom();
					child->borderBox.content.y = std::max(child->borderBox.content.y, newHeight);
				}
				else 
				{
					float newWidth = line.crossAxisSize - margin.Left() - margin.Right();
					child->borderBox.content.x = std::max(child->borderBox.content.x, newWidth);
				}
			}
				break;
			default:
				break;
			}

			// Compute position
			float mainPos = currentMain + (IsHorizontal() ? margin.Left() : margin.Top());
			crossPos += (IsHorizontal() ? margin.Top() : margin.Left());

			ArVec2 pos = MakeVec(mainPos, crossPos);
			child->borderBox.localPosition = pos;

			// Compute size
			if (extraSpace > 0.f && totalFlexGrow > 0.f)
			{
				ArVec2& size = child->borderBox.content;
				float basis = flexItem->flexBasis;

				basis += flexItem->flexGrow / totalFlexGrow * extraSpace;

				SelectMain(size.x, size.y) = basis;
			}

			currentMain += childMainSize + GetMainMargin(margin) + gap;
		}

		crossOffset += line.crossAxisSize + crossSpacing;
	}
}

void ArGraphicPrimRenderListElement::Awake(ArgonContext& context)
{
	borderBox.overflow = true;
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
		element->FrameUpdate(context);
	}
}

void ArGraphicLayer::EndFrame(ArgonContext& context)
{
	if (anyElementDead)
	{
		std::stack<ArGraphicElement*> stack = {};
		std::for_each(elements.begin(), elements.end(), [&stack](ArGraphicElement* element) { stack.push(element); });

		while (!stack.empty())
		{
			ArGraphicElement* element = stack.top();
			stack.pop();

			if (ArHasFlag(element->flags, ArGraphicElementFlag::Dead))
			{
				if (element->owner)
				{
					element->owner->beingState |= ArGraphicElementBeingState::ChildRemoved;
					element->owner->children.erase(std::remove(element->owner->children.begin(), element->owner->children.end(), element), element->owner->children.end());
				}
				else
				{
					elements.erase(std::remove(elements.begin(), elements.end(), element), elements.end());
				}
				delete element;
			}
			else
			{
				for (const auto& child : element->children)
				{
					stack.push(child);
				}
			}
		}

		anyElementDead = false;
	}
}

void ArGraphicLayer::UpdateFocus(ArgonContext& context)
{
	ArGraphicElement* currentHoveringElement = HitTest(context.inputManager.GetMousePosition());
	if (!lockFocus)
	{
		if (currentHoveringElement != hoveringElement)
		{
			hoveringElement = currentHoveringElement;
		}
		if (context.inputManager.IsMouseButtonDown(ArMouseButton::Left))
		{
			if (hoveringElement)
				SetFocus(hoveringElement);
			else
				SetFocus(nullptr);
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

		if (ArHasFlag(element->flags, ArGraphicElementFlag::Dead) ||
			ArLacksAnyFlags(element->flags, ArGraphicElementFlag::Interactive, ArGraphicElementFlag::Visible))
			continue;

		if (!element->HitTest(pos))
			continue;

		if (ArHasFlag(element->flags, ArGraphicElementFlag::Focusable) && element->depth > topMostDepth)
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
	if (element)
	{
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
			SetFocus(layer->focusingElement);
		}
		if (layer->hoveringElement)
		{
			foundHover = true;
			SetHover(layer->hoveringElement);
		}
		break;
	}
	if (!foundFocus)
	{
		SetFocus(nullptr);
	}
	if (!foundHover)
	{
		SetHover(nullptr);
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

void ArgonGraphicManager::SetFocus(ArGraphicElement* element)
{
	if (element == focusingElement)
		return;
	if (focusingElement)
	{
		focusingElement->focusing = false;
		focusingElement->beingState |= ArGraphicElementBeingState::Unfocus;
	}
	if (element)
	{
		element->focusing = true;
		element->beingState |= ArGraphicElementBeingState::Focus;
	}
	focusingElement = element;
}

void ArgonGraphicManager::SetHover(ArGraphicElement* element)
{
	if (element == hoveringElement)
		return;
	if (hoveringElement)
	{
		hoveringElement->hovering = false;
		hoveringElement->beingState |= ArGraphicElementBeingState::Unhover;
	}
	if (element)
	{
		element->hovering = true;
		element->beingState |= ArGraphicElementBeingState::Hover;
	}
	hoveringElement = element;
}
