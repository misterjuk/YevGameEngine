#pragma once
#include <glm.hpp>
#include "Component.h"
#include <memory>

namespace dae
{
	class GameObject;

	class TransformComponent final : public Component
	{
	public:
		TransformComponent(GameObject* owner);



		void SetLocalPosition(const float x, const float y, const float z);
		void SetLocalPosition(const glm::vec3& newPosition);

		void Translate(glm::vec3 const& offset);


		glm::vec3 GetLocalPosition() const;

		glm::vec3 GetWorldPosition();

		void UpdateWorldPosition();

		void SetPositionDirty(const bool isPositionDirty);
	private:
		bool m_positionIsDirty{ true };

		glm::vec3 m_position{};

		glm::vec3 m_worldPosition{};
	};

};
