#pragma once
#include <ArgonCore.h>

class SnowFlakeManager final : public ArGraphicElement
{
private:
	class SnowFlake : public ArGraphicElement
	{
	public:
		float fallSpeed = 0.f;
		float size = 0.f;
		float rotation = 0.f;
		float rotationSpeed = 0.f;
		float alpha = 0.f;
		float meltingSpeed = 0.f;

		std::vector<ArVec2> points = std::vector<ArVec2>(4);

		SnowFlake(float fallSpeed, float size, float rotation, float rotationSpeed, float alpha, float meltingSpeed = 0.f) :
			fallSpeed(fallSpeed), size(size), rotation(rotation), rotationSpeed(rotationSpeed), alpha(alpha), meltingSpeed(meltingSpeed) {
		}

		void Awake(const ArgonContext& context) override
		{
			interactive = false;

			boundingBox.position.x = ArHelp::Random::GetFloat(0.f, context.inputManager.GetDisplayState().size.x);
			boundingBox.position.y = -10.f;
			boundingBox.size = ArVec2(size, size);
		}

		void OnUpdate(const ArgonContext& context) override
		{
			dead = (boundingBox.position.y >= context.inputManager.GetDisplayState().size.y) || alpha <= 0.f;

			rotation += rotationSpeed * context.GetDeltaTime();

			alpha -= meltingSpeed * context.GetDeltaTime();

			ArVec2 mousePos = context.inputManager.GetMousePosition();

			float distance = (mousePos - boundingBox.position).Length();
			if (distance <= 100.f)
				boundingBox.position += (mousePos - boundingBox.position).Normalized() * fallSpeed * context.GetDeltaTime();
			else
				boundingBox.position.y += fallSpeed * context.GetDeltaTime();

			// 绕中心旋转
			ArVec2 center = boundingBox.GetRect().GetCenter();
			//sin, cos
			float sin = sinf(rotation);
			float cos = cosf(rotation);
			points[0] = center + ArVec2(-size / 2.f * cos - size / 2.f * sin, -size / 2.f * sin + size / 2.f * cos);
			points[1] = center + ArVec2(size / 2.f * cos - size / 2.f * sin, size / 2.f * sin + size / 2.f * cos);
			points[2] = center + ArVec2(size / 2.f * cos + size / 2.f * sin, size / 2.f * sin - size / 2.f * cos);
			points[3] = center + ArVec2(-size / 2.f * cos + size / 2.f * sin, -size / 2.f * sin - size / 2.f * cos);
		}

		void OnRender(const ArgonContext& context) override
		{
			if (dead)
				return;

			renderList->AddConvexPolyFilled(points,
				AR_COL32(255.f, 255.f, 255.f, alpha)
			);
		}
	};
public:
	float deltaTimeAccumulator = 0.f;
	uint32_t snowfallCountPerHalfSecond = 10;
public:
	void Awake(const ArgonContext& context) override
	{
		interactive = focusable = visible = false;
	}

	void OnUpdate(const ArgonContext& context) override
	{
		deltaTimeAccumulator += ArGui::GetContext().GetDeltaTime();
		if (deltaTimeAccumulator >= 0.5f)
		{
			deltaTimeAccumulator = 0.f;

			for (size_t i = 0; i < snowfallCountPerHalfSecond; i++)
			{
				float fallSpeed = ArHelp::Random::GetFloat(50.f, 100.f);
				float size = ArHelp::Random::GetFloat(5.f, 15.f);
				float rotation = ArHelp::Random::GetFloat(0.f, 2 * AR_PI);
				float rotationSpeed = ArHelp::Random::GetFloat(-1.f, 1.f);
				float alpha = ArHelp::Random::GetFloat(100.f, 255.f);
				float meltingSpeed = ArHelp::Random::GetFloat(0.f, 50.f);
				layer->AddElement(new SnowFlake(fallSpeed, size, rotation, rotationSpeed, alpha, meltingSpeed));
			}
		}
	}
};

//inline auto snowFlakeManager = ArGui::GetGraphicManager().AddLayer(ArLayerAdditionPriority::AfterBackground)->AddElement(new SnowFlakeManager());
