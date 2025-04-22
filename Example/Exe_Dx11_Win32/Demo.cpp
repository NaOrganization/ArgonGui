#include "Demo.h"
#include <Backends/ArgonDx11Renderer.h>

class ArGDroppableComp : public IArGraphicComponent
{
private:
	bool isDragging = false;
	ArVec2 dragOffset = ArVec2(0.f, 0.f);
	ArGraphicElement* rootElement = nullptr;
public:
	void Awake(ArGraphicElement& owner) override
	{
		rootElement = owner.GetRootElement();
	}

	void OnUpdate(ArGraphicElement& owner, const ArgonContext& context) override
	{
		if (!owner.focusing || !owner.hovered)
			return;
		const ArgonInputManager& inputManager = context.inputManager;
		ArVec2 currentMousePos = inputManager.GetMousePosition();
		if (inputManager.IsMouseButtonDown(ArMouseButton::Left))
		{
			isDragging = true;
			dragOffset = currentMousePos - rootElement->boundingBox.GetActualPosition();
			owner.layer->LockFocus();
		}
		if (!isDragging)
			return;
		if (!inputManager.IsMouseButtonHeld(ArMouseButton::Left))
		{
			isDragging = false;
			dragOffset = ArVec2(0.f, 0.f);
			owner.layer->UnlockFocus();
			return;
		}
		rootElement->boundingBox.position = currentMousePos - dragOffset;
	}
};

class ArGResizableElement : public ArGraphicElement
{
private:
	bool isResizing = false;
	ArVec2 dragOffset = ArVec2(0.f, 0.f);

	ArVec2 minSize = {};
	ArGPolygonCollisionComp* mouseCollision = nullptr;
public:
	ArGResizableElement(ArVec2 minSize) : minSize(minSize) {}

	void Awake(const ArgonContext& context) override
	{
		depth = root->FindMostDeepestChild() + 1;
		const float padding = 3.f;
		const float size = 10.f;
		const float size2 = 3.f;
		boundingBox.size = ArVec2(size, size);

		mouseCollision = new ArGPolygonCollisionComp(std::vector<ArVec2>({
			ArVec2(size, size),
			ArVec2(0.f, size),
			ArVec2(0.f, size - size2),
			ArVec2(size - size2, size - size2),
			ArVec2(size - size2, 0.f),
			ArVec2(size, 0.f),
			}));
		AddComponent(typeid(ArGPolygonCollisionComp), mouseCollision);
	}

	void OnUpdate(const ArgonContext& context) override
	{
		const float padding = 5.f;
		boundingBox.localPosition = ArVec2(
			owner->boundingBox.size.x - boundingBox.size.x - padding,
			owner->boundingBox.size.y - boundingBox.size.y - padding
		);

		if (!focusing || !hovered)
			return;

		const ArgonInputManager& inputManager = context.inputManager;
		ArVec2 currentMousePos = inputManager.GetMousePosition();
		if (mouseCollision->hovering)
		{
			if (!inputManager.IsMouseButtonDown(ArMouseButton::Left))
				return;
			isResizing = true;
			dragOffset = currentMousePos - owner->boundingBox.size;
			layer->LockFocus();
		}
		if (!isResizing)
			return;
		if (!inputManager.IsMouseButtonHeld(ArMouseButton::Left))
		{
			isResizing = false;
			dragOffset = ArVec2(0.f, 0.f);
			layer->UnlockFocus();
			return;
		}
		owner->boundingBox.size = ArMax(minSize, currentMousePos - dragOffset);
	}

	void OnRender(const ArgonContext& context) override
	{
		std::vector<ArVec2> bounds = mouseCollision->bounds;
		for (auto& point : bounds)
		{
			point += boundingBox.GetActualPosition();
		}
		renderList->AddConvexPolyFilled(bounds, AR_COL32(255.f, 255.f, 255.f, 255.f));
	}

	bool HitTest(const ArVec2& pos) const override
	{
		return mouseCollision->hovering;
	}
};

class IArGLayoutComp : public IArGraphicComponent
{
};

class ArGHorizontalLayoutComp : public IArGLayoutComp
{
public:
	void OnUpdate(ArGraphicElement& owner, const ArgonContext& context) override
	{
		float offsetX = 5.f;
		for (auto& child : owner.children)
		{
			child->boundingBox.localPosition = ArVec2(offsetX, 5.f);
			offsetX += child->boundingBox.size.x + 5.f;
		}
	}
};

class ArGVerticalLayoutComp : public IArGLayoutComp
{
public:
	void OnUpdate(ArGraphicElement& owner, const ArgonContext& context) override
	{
		float offsetY = 5.f;
		for (auto& child : owner.children)
		{
			if (!child->visible)
				continue;
			child->boundingBox.localPosition = ArVec2(5.f, offsetY);
			offsetY += child->boundingBox.size.y + 5.f;
		}
	}
};

class SimpleWindowElement : public ArGraphicElement
{
public:
	class TitleBar : public ArGraphicElement
	{
	public:
		ArStringView title = {};

		TitleBar(ArStringView title) : title(title) {}

		void Awake(const ArgonContext& context) override
		{
			AddComponent(typeid(ArGDroppableComp), new ArGDroppableComp);

			boundingBox.size = ArVec2(owner->boundingBox.size.x, 30.f);
		}

		void OnUpdate(const ArgonContext& context) override
		{
			boundingBox.size = ArVec2(owner->boundingBox.size.x, 30.f);
		}

		void OnRender(const ArgonContext& context) override
		{
			renderList->AddRectFilled(boundingBox.GetRect(), AR_COL32(140.f, 140.f, 140.f, 255.f));
			renderList->AddText(title, 16, boundingBox.GetActualPosition() + ArVec2(5.f, 3.f), AR_COL32_WHITE);
			renderList->AddRect(boundingBox.GetRect(), AR_COL32(0.f, 0.f, 0.f, 255.f), 1.f);
		}
	};
	class Container : public ArGraphicElement
	{
	public:
		class CheckBox : public ArGraphicElement
		{
		public:
			ArGFloatAnimatorComp* animation = nullptr;
			bool* checkedValue = nullptr;
			ArStringView title = {};

			CheckBox(ArStringView title, bool* checkedValue = nullptr) : title(title), checkedValue(checkedValue) {}

			void Awake(const ArgonContext& context) override
			{
				animation = new ArGFloatAnimatorComp(*checkedValue ? 255.f : 0.f);
				AddComponent(typeid(ArGFloatAnimatorComp), animation);

				boundingBox.size = ArVec2(context.renderManager.textureManager.CalcTextSize(title, 16u).x + 30.f, 20.f);
			}

			void OnUpdate(const ArgonContext& context) override
			{

				if (!focusing)
					return;

				if ((hovered && context.inputManager.IsMouseButtonDown(ArMouseButton::Left)) || context.inputManager.IsGamepadButtonDown(ArGamepadButton::FaceDown))
				{
					*checkedValue = !*checkedValue;
					animation->StartNewProcess(*checkedValue ? 0.f : 255.f, *checkedValue ? 255.f : 0.f, 200ms);
					
				}
			}

			void OnRender(const ArgonContext& context) override
			{
				renderList->AddRectFilled(ArRect(boundingBox.GetRect().min, boundingBox.GetRect().min + ArVec2(20.f, 20.f)), AR_COL32(130.f, 130.f, 130.f, 255.f));
				renderList->AddRectFilled(ArRect(boundingBox.GetRect().min + ArVec2(5.f, 5.f), boundingBox.GetRect().min + ArVec2(15.f, 15.f)), AR_COL32(20.f, 20.f, 20.f, animation->value));
				renderList->AddText(title, 16, boundingBox.GetActualPosition() + ArVec2(25.f, 2.f), AR_COL32_WHITE);
			}
		};

		void Awake(const ArgonContext& context) override
		{
			focusable = false;

			boundingBox.size = ArVec2(owner->boundingBox.size.x, owner->boundingBox.size.y - 30.f);
			boundingBox.localPosition = ArVec2(0.f, 30.f);

			AddComponent(typeid(ArGVerticalLayoutComp), new ArGVerticalLayoutComp); // Add vertical layout

			bool* texture = new bool(false);
			CheckBox* checkBox3 = new CheckBox("None", texture);
			CheckBox* checkBox2 = new CheckBox("Enable cb3", &checkBox3->visible);
			CheckBox* checkBox1 = new CheckBox("Enable cb2", &checkBox2->visible);

			AddChild(checkBox1);
			AddChild(checkBox2);
			AddChild(checkBox3);
		}

		void OnUpdate(const ArgonContext& context) override
		{
			boundingBox.size = ArVec2(owner->boundingBox.size.x, owner->boundingBox.size.y - 30.f);
		}
	};
public:
	TitleBar* titleBar = nullptr;
	SimpleWindowElement(ArStringView title) : titleBar(new TitleBar(title)) {}

	void Awake(const ArgonContext& context) override
	{
		AddChild(titleBar);
		AddChild(new Container());
		AddChild(new ArGResizableElement(ArVec2(50.f, 50.f)));

		boundingBox.position = ArVec2(200.f, 200.f);
		boundingBox.size = ArVec2(300.f, 200.f);
	}

	void OnRender(const ArgonContext& context) override
	{
		renderList->AddRectFilled(ArRect(boundingBox.GetRect().min + ArVec2(0.f, 30.f), boundingBox.GetRect().max), AR_COL32(100.f, 100.f, 100.f, 255.f));
		renderList->AddRect(boundingBox.GetRect(), AR_COL32(0.f, 0.f, 0.f, 255.f), 1.f);
	}
};

class FocusSelectorTestElement : public ArGraphicElement
{
public:
	bool selected = false;
	ArGVec2AnimatorComp* positionAnimation = nullptr;
	ArGVec2AnimatorComp* sizeAnimation = nullptr;
	ArGFloatAnimatorComp* alphaAnimation = nullptr;
	ArGraphicBoundingBox lastFocusBoundingBox = {};
	ArGraphicElement* lastFocusElement = nullptr;

	void Awake(const ArgonContext& context) override
	{
		focusable = interactive = false;

		alphaAnimation = new ArGFloatAnimatorComp(255.f, [](float t) { return t; });
		positionAnimation = new ArGVec2AnimatorComp(ArVec2(0.f, 0.f), [](float t) { return ArVec2(t, t); });
		sizeAnimation = new ArGVec2AnimatorComp(ArVec2(0.f, 0.f), [](float t) { return ArVec2(t, t); });

		AddComponent(typeid(ArGFloatAnimatorComp), alphaAnimation);
		AddComponent(typeid(ArGVec2AnimatorComp), positionAnimation);
		AddComponent(typeid(ArGVec2AnimatorComp), sizeAnimation);

		boundingBox.size = context.inputManager.GetDisplayState().size;
	}

	void OnUpdate(const ArgonContext& context) override
	{
		if (selected && context.graphicManager.focusingElement == nullptr)
		{
			alphaAnimation->StartNewProcess(255.f, 0.f, 500ms);
		}
		else if (!selected && context.graphicManager.focusingElement != nullptr)
		{
			alphaAnimation->StartNewProcess(0.f, 255.f, 500ms);
			boundingBox.position = context.graphicManager.focusingElement->boundingBox.GetActualPosition() - ArVec2(5.f, 5.f);
			boundingBox.size = context.graphicManager.focusingElement->boundingBox.size + ArVec2(10.f, 10.f);
		}

		selected = context.graphicManager.focusingElement != nullptr;

		if (!selected)
			return;

		if (lastFocusBoundingBox != context.graphicManager.focusingElement->boundingBox)
		{
			ArVec2 size = context.graphicManager.focusingElement->boundingBox.size + ArVec2(10.f, 10.f);
			ArVec2 position = context.graphicManager.focusingElement->boundingBox.GetActualPosition() - ArVec2(5.f, 5.f);

			if (lastFocusElement != context.graphicManager.focusingElement)
			{
				positionAnimation->StartNewProcess(boundingBox.position, position, 200ms);
			}
			else
			{
				positionAnimation->value = position;
			}

			sizeAnimation->StartNewProcess(boundingBox.size, size, 500ms);
		}

		lastFocusElement = context.graphicManager.focusingElement;
		lastFocusBoundingBox = context.graphicManager.focusingElement->boundingBox;

		boundingBox.position = positionAnimation->value;
		boundingBox.size = sizeAnimation->value;
	}

	void OnRender(const ArgonContext& context) override
	{
		renderList->AddRect(boundingBox.GetRect(), AR_COL32(255.f, 255.f, 255.f, alphaAnimation->value), 2.f);
	}
};

auto a = ArGui::GetGraphicManager().GetDefaultLayer();

auto testElementInstance = a->AddElement(new SimpleWindowElement("Demo Window 1"));
auto testElementInstance2 = a->AddElement(new SimpleWindowElement("Demo Window 2"));
auto testElementInstance3 = a->AddElement(new SimpleWindowElement("Demo Window 3"));
auto testElementInstance4 = a->AddElement(new SimpleWindowElement("Demo Window 4"));
auto testElementInstance5 = a->AddElement(new SimpleWindowElement("Demo Window 5"));

//auto focusSelector = ArGui::GetGraphicManager().AddLayer(ArLayerAdditionPriority::BeforeForeground)->AddElement(new FocusSelectorTestElement());
