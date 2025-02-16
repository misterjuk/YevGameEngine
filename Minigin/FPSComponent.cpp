#include "FPSComponent.h"
#include "Time.h"
#include "TextComponent.h"
#include "GameObject.h"
namespace dae 

{
    FPSComponent::FPSComponent(GameObject* owner)
        : Component(owner)
    {

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

            if (const auto gameObject = m_Owner)
            {
                if (const auto textComponent = gameObject->GetComponent<TextComponent>(); textComponent != nullptr)
                {
                    textComponent->SetText(m_text);
                }
            }
        }
    }

}
