#include "ScoreDisplayComponent.h"
#include "TextComponent.h"
#include "GameEvents.h"
#include "GameObject.h"
#include "ScoreComponent.h"
#include "GameManager.h"

ScoreDisplayComponent::ScoreDisplayComponent(yev::GameObject* ownerObjectPtr, yev::TextComponent* textComponent)
	: yev::Component(ownerObjectPtr), m_ScoreText{ textComponent }
{
	//BAD WORKAROUND
	UpdateScoreDiplay(GameManager::GetInstance().GetPlayerScore());
}

void ScoreDisplayComponent::UpdateScoreDiplay(int currentScore)
{
	m_ScoreText->SetText("Score: " + std::to_string(currentScore));
}

void ScoreDisplayComponent::ForceScoreUpdate()
{
	//BAD WORKAROUND
	UpdateScoreDiplay(GameManager::GetInstance().GetPlayerScore());
}

void ScoreDisplayComponent::Notify(Event event, yev::GameObject* actor)
{
	if (event == GameEvents::ScoreChanged)
	{
		if (actor)
		{			
			UpdateScoreDiplay(actor->GetComponent<ScoreComponent>()->GetCurrentScore());
		}

	}
}
