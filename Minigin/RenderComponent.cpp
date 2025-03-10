#include "RenderComponent.h"
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "TransformComponent.h"
#include "Texture2D.h"

namespace yev 
{
    RenderComponent::RenderComponent(GameObject* owner)
        : RenderComponent(owner, nullptr)
    {
        
    }

    RenderComponent::RenderComponent(GameObject* owner, const std::shared_ptr<Texture2D>& texture)
        : Component(owner), m_texture(texture)
    {
        m_transform = owner->GetComponent<TransformComponent>();    
    }


    void RenderComponent::Render() const
    {
        if (m_texture != nullptr)
        {
            if (m_transform != nullptr)
            {
                Renderer::GetInstance().RenderTexture(*m_texture, m_transform->GetWorldPosition().x, m_transform->GetWorldPosition().y,
                    m_texture->GetSize().x * m_transform->GetScale().x, m_texture->GetSize().y * m_transform->GetScale().y);
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
    }
}
