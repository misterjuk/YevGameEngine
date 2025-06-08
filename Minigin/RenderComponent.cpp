#include "RenderComponent.h"
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "TransformComponent.h"
#include "Texture2D.h"

namespace yev 
{
    RenderComponent::RenderComponent(GameObject* owner)
        : Component(owner)
    {
        m_transform = owner->GetComponent<TransformComponent>();
    }

    RenderComponent::RenderComponent(GameObject* owner, const std::shared_ptr<Texture2D>& texture)
        : Component(owner), m_texture(texture)
    {
        m_transform = owner->GetComponent<TransformComponent>();    
    }

    RenderComponent::RenderComponent(GameObject* owner, const std::string& texturePath)
        : Component(owner)
    {
        m_transform = owner->GetComponent<TransformComponent>();
        m_texture = ResourceManager::GetInstance().LoadTexture(texturePath);
    }


    void RenderComponent::Render() const
    {
        if (m_texture != nullptr)
        {
            if (m_transform != nullptr)
            {
                const glm::vec3& worldPos = m_transform->GetWorldPosition();
                Renderer::GetInstance().RenderTexture(*m_texture, 
                                                     worldPos.x, worldPos.y,
                                                     m_texture->GetSize().x * m_transform->GetScale().x, 
                                                     m_texture->GetSize().y * m_transform->GetScale().y);
            }
            else
            {
                Renderer::GetInstance().RenderTexture(*m_texture, 0, 0);
            }
        }
    }

    void RenderComponent::SetTexture(const std::string& filename)
    {
        m_texture = ResourceManager::GetInstance().LoadTexture(filename);

        if(m_transform == nullptr)
        {
            m_transform = m_Owner->GetComponent<TransformComponent>();
		}
    }
}
