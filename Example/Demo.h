#pragma once
#include <ArgonGui.h>

namespace ArgonDemo
{
	class DemoStyleProperties : public IArGraphicStyleCollection
	{
	public:
		std::unordered_map<ArStringView, ArGraphicStyleProperty> Properties() const override
		{
			return{
				{ "titleBar_text_align", { ArVec2(0.5f, 0.5f) }}
			};
		}
	};

	class DemoColorPalette : public IArGraphicColorPalette
	{
	public:
		std::unordered_map<ArStringView, ArColor> Colors() const override
		{
			return {
				{"window_background", AR_COL32(100.f, 100.f, 100.f, 255.f)},
				{"window_border", AR_COL32(50.f, 50.f, 50.f, 255.f)},
				{"titleBar_background", AR_COL32(140.f, 140.f, 140.f, 255.f)},
			};
		}
	};

	class TitleBar : public ArGraphicElement
	{
	public:
		ArString title = "Title Bar";

		void Awake(ArgonContext& context) override
		{
			borderBox.content.x = owner->borderBox.content.x;
			borderBox.content.y = 30.f;

			AddComponent(new ArDroppableComp(owner));
		}

		void OnUpdate(ArgonContext& context) override
		{
			borderBox.content.x = owner->borderBox.content.x;
		}

		void OnRender(ArgonContext& context) override
		{
			ArVec2 titleSize = context.renderManager.CalcTextSize(title, 16u);

			renderList->AddRectFilled(borderBox.GetContentRect(), context.themeManager.GetColor("titleBar_background"));
			renderList->AddText(title, 16u, borderBox.GetContentPosition() + ((borderBox.content - titleSize) * (ArVec2)context.themeManager.GetProperty("titleBar_text_align")), AR_COL32_WHITE);
		}
	};

	class Card : public ArGraphicElement
	{
	public:
		ArVec2 size = ArVec2(200.f, 200.f);
		Card(ArVec2 size = ArVec2(200.f, 200.f)) : size(size) {}

		void Awake(ArgonContext& context) override
		{
			flags |= ArGraphicElementFlag::Debug;
			borderBox.content = size;
			borderBox.borderSize = 1.f;
		}

		void OnRender(ArgonContext& context) override
		{
			renderList->AddRectFilled(borderBox.GetContentRect(), AR_COL32(255.f, 0.f, 0.f, 255.f));
			renderList->AddRect(borderBox.GetBorderRect(), AR_COL32(0.f, 0.f, 0.f, 255.f), borderBox.borderSize);
		}
	};

	class SimpleWindow : public ArGraphicElement
	{
	public:
		void Awake(ArgonContext& context) override
		{
			borderBox.position = ArVec2(10.f, 10.f);
			borderBox.content = ArVec2(700.f, 400.f);
			borderBox.borderSize = 1.f;

			AddComponent(new ArFlexLayoutComp());

			AddChild(new TitleBar());

			class Container : public ArGraphicElement
			{
			public:
				void Awake(ArgonContext& context) override
				{
					flags |= ArGraphicElementFlag::Debug;
					borderBox.content.x = owner->borderBox.content.x;

					AddComponent(new ArFlexItemComp(1.f));
					AddComponent(new ArFlexLayoutComp(
						ArFlexLayoutDirection::Horizontal, 
						ArFlexLayoutJustifyContent::FlexStart, 
						ArFlexLayoutAlignItems::FlexStart, 
						ArFlexLayoutWrap::Wrap, 
						ArFlexLayoutAlignContent::FlexStart)
					);

					// 5个大小不一的卡片
					AddChild(new Card(ArVec2(100.f, 100.f)));
					AddChild(new Card(ArVec2(200.f, 200.f)));
					AddChild(new Card(ArVec2(150.f, 150.f)));
					AddChild(new Card(ArVec2(300.f, 300.f)));
					AddChild(new Card(ArVec2(50.f, 50.f)));
				}

				void OnUpdate(ArgonContext& context) override
				{
					borderBox.content.x = owner->borderBox.content.x;
				}
			};
			AddChild(new Container());
		}

		void OnUpdate(ArgonContext& context) override
		{
		}

		void OnRender(ArgonContext& context) override
		{
			renderList->AddRectFilled(borderBox.GetContentRect(), context.themeManager.GetColor("window_background"));
			renderList->AddRect(borderBox.GetBorderRect(), context.themeManager.GetColor("window_border"), 1.f);
		}
	};

	void Setup()
	{
		ArGui::GetThemeManager().SetDefualProperties(DemoStyleProperties());
		ArGui::GetThemeManager().SetDefualtColors(DemoColorPalette());

		ArGui::GetGraphicManager().GetDefaultLayer()->AddElement(new SimpleWindow());
	}
}