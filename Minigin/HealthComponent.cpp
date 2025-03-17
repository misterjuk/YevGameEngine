#include "HealthComponent.h"
#include "GameObject.h"
#include "SceneManager.h"
#include "GameEvents.h"

HealthComponent::HealthComponent(yev::GameObject* owner)
	:Component(owner)
{
}



void HealthComponent::Notify(IObserver::Event event, yev::GameObject* gameObject)
{
	if (event == GameEvents::PlayerDamaged)
	{
		--m_Health;

		NotifyObservers(GameEvents::PlayerDamaged, gameObject);
	
		if (m_Health <= 0)
		{
			
		}
	}

}


