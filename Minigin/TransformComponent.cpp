#include "TransformComponent.h"


namespace dae 

{

	TransformComponent::TransformComponent(GameObject* owner) :
		Component(owner)
	{
	}


	void TransformComponent::SetPosition(const float x, const float y, const float z)
	{
		m_position.x = x;
		m_position.y = y;
		m_position.z = z;
	}

}
