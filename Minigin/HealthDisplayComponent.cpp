#include "HealthDisplayComponent.h"
#include "TextComponent.h"
#include "GameEvents.h"
#include "GameObject.h"
#include "HealthComponent.h"
HealthDisplayComponent::HealthDisplayComponent(yev::GameObject* ownerObjectPtr, yev::TextComponent* textComponent)
	: yev::Component(ownerObjectPtr), m_HealthText{ textComponent }
{
	UpdateHealthDiplay(3);
}

void HealthDisplayComponent::UpdateHealthDiplay(int currentHealth)
{
	m_HealthText->SetText("Health: " + std::to_string(currentHealth));
}

void HealthDisplayComponent::Notify(Event event, yev::GameObject* actor)
{
	if (event == GameEvents::PlayerDamaged)
	{
		if (actor)
		{
			//maybe pass health as a parameter
			UpdateHealthDiplay(actor->GetComponent<HealthComponent>()->GetHealth());
		}
	
	}
}
