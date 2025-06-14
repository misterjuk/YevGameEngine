#include "ScoreComponent.h"
#include <fstream>
#include <iostream>
#include "GameObject.h"
#include "TextComponent.h"
#include "GameEvents.h"
#include "GameManager.h"

ScoreComponent::ScoreComponent(yev::GameObject* ownerObjectPtr)
    : yev::Component(ownerObjectPtr), m_currentScore(0)
{

   
}

void ScoreComponent::AddScore(int score)
{

    m_currentScore += score;
	//m_ScoreText->SetText("Score: " + std::to_string(m_currentScore));

	//std::cout << "Score: " << m_currentScore << std::endl;

	//BAD WORKAROUND
	GameManager::GetInstance().SetPlayerScore(m_currentScore);
    
	NotifyObservers(GameEvents::ScoreChanged, m_Owner);
}


void ScoreComponent::Notify(Event, yev::GameObject*)
{
    /*if (event == GameEvents::PlayerScored)
    {
        AddScore(10);
        NotifyObservers(GameEvents::PlayerScored, gameObject);
    }*/
	
}

void ScoreComponent::SetScore(int score)
{
    m_currentScore = score; 

    //BAD WORKAROUND
    GameManager::GetInstance().SetPlayerScore(m_currentScore);

    NotifyObservers(GameEvents::ScoreChanged, m_Owner); 
}


