#include <string>
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"

namespace dae 
{
	GameObject::~GameObject() = default;

	void GameObject::Update()
	{

		for (auto& component : m_Components)
		{
			if (!component->IsMarkedForDeletion())
			{
				component->Update();
			}
		}

		// Remove components that were marked for deletion
		m_Components.erase(
			std::remove_if(m_Components.begin(), m_Components.end(),
				[](const std::unique_ptr<Component>& comp) { return comp->IsMarkedForDeletion(); }),
			m_Components.end());
	}

	void GameObject::Render() const
	{
		for (const auto& component : m_Components)
		{
			component->Render();
		}
	}

}

