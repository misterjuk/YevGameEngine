#pragma once
#include "Component.h"
#include <glm.hpp>

namespace yev 
{

	class TransformComponent;

	class RotatorComponent final : public Component
	{
	public:

		RotatorComponent(GameObject* owner);
		~RotatorComponent() = default;

		void Update() override;

		void SetRadius(const float radius) { m_Radius = radius; };
		void SetSpeed(const float speed) { m_Speed = speed; };

	private:
		float m_Radius{20.0f}; 
		float m_Speed{1.0f};  

		float m_Angle{};

		glm::vec3 m_TargetPosition{};

		TransformComponent* m_TransformComponent;
	};

}

