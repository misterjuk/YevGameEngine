#include <string>
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "TransformComponent.h"

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

	void GameObject::SetParent(GameObject* parent, bool keepWorldPosition)
	{
        if (IsChild(parent) || parent == this || m_Parent == parent)
        {
            return;
        }

        // Remove from the current parent
        if (m_Parent)
        {
            m_Parent->RemoveChild(this);
        }

        if (parent == nullptr)
        {
            m_Parent = nullptr;
            GetComponent<TransformComponent>()->SetLocalPosition(GetComponent<TransformComponent>()->GetWorldPosition());
        }
        else
        {
            if (keepWorldPosition)
            {
                GetComponent<TransformComponent>()->SetLocalPosition(
                    GetComponent<TransformComponent>()->GetWorldPosition() -
                    parent->GetComponent<TransformComponent>()->GetWorldPosition());
            }

            SetPositionDirty(true);

            m_Parent = parent;
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

