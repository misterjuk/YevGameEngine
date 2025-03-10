#include <string>
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "TransformComponent.h"

namespace yev 
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

	void GameObject::SetParent(GameObject* parent, bool keepWorldPosition)
	{
        // Check for invalid parent relationships
        if (IsChild(parent) || parent == this || m_Parent == parent)
        {
            return;
        }

        // Remove from current parent if it exists
        if (m_Parent)
        {
            m_Parent->RemoveChild(this);
        }

        // Set new parent
        m_Parent = parent;

        // Handle local position based on the keepWorldPosition flag
        auto transform = GetComponent<TransformComponent>();
        glm::vec3 newLocalPosition;

        if (keepWorldPosition)
        {
            if (parent)
            {
                glm::vec3 newWorldPosition = transform->GetWorldPosition();
                glm::vec3 parentWorldPosition = parent->GetComponent<TransformComponent>()->GetWorldPosition();
                newLocalPosition = newWorldPosition - parentWorldPosition;
            }
            else
            {
                newLocalPosition = transform->GetWorldPosition(); // Keep world position when detaching
            }
        }
        else
        {
                newLocalPosition = glm::vec3(0.0f, 0.0f, 0.0f);       
        }

        transform->SetLocalPosition(newLocalPosition);

        // Mark as dirty if position was reset or a new parent is assigned
        if (!keepWorldPosition || parent)
        {
            SetPositionDirty(true);
        }

        // Add this object as a child to the new parent if it exists
        if (parent)
        {
            m_Parent->AddChild(this);
        }
	}

    bool GameObject::IsChild(GameObject* childToCheck) const
    {
        for (const auto& child : m_Children)
        {
            if (child == childToCheck || child->IsChild(childToCheck))
            {
                return true;
            }
        }
        return false;
    }

    void GameObject::AddChild(GameObject* child)
    {
        if (child && !IsChild(child))
        {
            m_Children.push_back(child);
        }
    }

    void GameObject::RemoveChild(GameObject* child)
    {

        m_Children.erase(
            std::remove(m_Children.begin(), m_Children.end(), child),
            m_Children.end()
        );
    }

    void GameObject::SetPositionDirty(const bool isPositionDirty)
    {

        //TOOD Set childs dirty
        GetComponent<TransformComponent>()->SetPositionDirty(isPositionDirty);

        for (auto* child : m_Children) // child is GameObject*
        {
            child->SetPositionDirty(isPositionDirty);
        }

    }

};

