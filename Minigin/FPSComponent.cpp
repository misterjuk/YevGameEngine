#include "FPSComponent.h"
#include "Time.h"
#include "TextComponent.h"
#include "GameObject.h"
namespace yev 

{
    FPSComponent::FPSComponent(GameObject* owner)
        : Component(owner)
    {
        if (m_Owner)
        {
            m_TextComponent = m_Owner->GetComponent<TextComponent>(); 
        }
    }

    void FPSComponent::Update()

    {
        const auto currentTime = std::chrono::steady_clock::now();
        if (const auto elapsedSeconds = std::chrono::duration<float>(currentTime - m_lastUpdateTime).count(); elapsedSeconds >= m_TimeBetweenUpdates)
        {
            m_lastUpdateTime = currentTime;

            std::stringstream ss;
            ss << std::fixed << std::setprecision(1) << 1 / Time::GetInstance().GetDeltaTime();
            m_text = "FPS: " + ss.str();

            if (m_TextComponent) // Use the cached reference
            {
                m_TextComponent->SetText(m_text);
            }
            else 
            {
                return;
            }
        }
    }

}
