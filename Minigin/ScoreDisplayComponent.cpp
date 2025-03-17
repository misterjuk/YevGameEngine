#include "ScoreDisplayComponent.h"
#include "TextComponent.h"
#include "GameEvents.h"
#include "GameObject.h"
#include "ScoreComponent.h"
ScoreDisplayComponent::ScoreDisplayComponent(yev::GameObject* ownerObjectPtr, yev::TextComponent* textComponent)
	: yev::Component(ownerObjectPtr), m_ScoreText{ textComponent }
{
	UpdateScoreDiplay(0);
}

void ScoreDisplayComponent::UpdateScoreDiplay(int currentScore)
{
	m_ScoreText->SetText("Score: " + std::to_string(currentScore));
}

void ScoreDisplayComponent::Notify(Event event, yev::GameObject* actor)
{
	if (event == GameEvents::PlayerScored)
	{
		if (actor)
		{
			//maybe pass health as a parameter
			UpdateScoreDiplay(actor->GetComponent<ScoreComponent>()->GetCurrentScore());
		}

	}
}
