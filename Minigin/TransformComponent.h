#pragma once
#include <glm.hpp>
#include "Component.h"


namespace dae
{
	class TransformComponent final : public Component
	{
	public:
		TransformComponent(GameObject* owner);
		

		const glm::vec3& GetPosition() const { return m_position; }
		void SetPosition(float x, float y, float z);
	private:
		glm::vec3 m_position;
	};
}
