#define _USE_MATH_DEFINES

#include "RotatorComponent.h"
#include "GameObject.h"
#include "TransformComponent.h"
#include "Time.h"
#include <cmath>   
#include <stdexcept>

namespace yev 
{
    RotatorComponent::RotatorComponent(GameObject* owner)
        : Component(owner)
    {
        // Cache the TransformComponent pointer
        if (auto transformComponent = owner->GetComponent<TransformComponent>())
        {
            m_TransformComponent = transformComponent;
            m_TargetPosition = m_TransformComponent->GetLocalPosition();
        }
        else
        {
            // Handle error: TransformComponent not found
            throw std::runtime_error("TransformComponent not found on the GameObject");
        }
    }

    void RotatorComponent::Update()
    {
        if (m_Owner == nullptr || m_TransformComponent == nullptr) 
            return;
       
        m_Angle += m_Speed * Time::GetInstance().GetDeltaTime();

        // Keep angle within [0, 2pi] to avoid overflow
        m_Angle = fmod(m_Angle, 2 * static_cast<float>(M_PI));

        glm::vec3 position;
        position.x = m_TargetPosition.x + m_Radius * std::cos(m_Angle);
        position.y = m_TargetPosition.y + m_Radius * std::sin(m_Angle);
        m_TransformComponent->SetLocalPosition(position);
    }
}
