#include "ScoreComponent.h"
#include <fstream>
#include <iostream>
#include "GameObject.h"
#include "TextComponent.h"
#include "GameEvents.h"

ScoreComponent::ScoreComponent(yev::GameObject* ownerObjectPtr)
    : yev::Component(ownerObjectPtr), m_currentScore(0), m_highScore(0)
{

    //LoadHighScore();

}

void ScoreComponent::AddScore(int score)
{

    m_currentScore += score;
	//m_ScoreText->SetText("Score: " + std::to_string(m_currentScore));

	//std::cout << "Score: " << m_currentScore << std::endl;
	NotifyObservers(GameEvents::ScoreChanged, m_Owner);
}

void ScoreComponent::SaveHighScore()
{
    std::ofstream file(m_highScoreFilePath);
    if (file.is_open())
    {
        file << m_highScore;
        file.close();
    }
    else
    {
        std::cerr << "Unable to open file for saving high score: " << m_highScoreFilePath << std::endl;
    }
}

void ScoreComponent::LoadHighScore()
{
    std::ifstream file(m_highScoreFilePath);
    if (file.is_open())
    {
        file >> m_highScore;

        //m_ScoreText->SetText("HighScore: " + std::to_string(m_highScore));
        file.close();
    }
    else
    {
        std::cerr << "Unable to open file for loading high score: " << m_highScoreFilePath << std::endl;
    }
}

void ScoreComponent::Notify(Event, yev::GameObject*)
{
    /*if (event == GameEvents::PlayerScored)
    {
        AddScore(10);
        NotifyObservers(GameEvents::PlayerScored, gameObject);
    }*/
	
}

void ScoreComponent::UpdateHighScore()
{
    if (m_currentScore > m_highScore)
    {
        m_highScore = m_currentScore;
        SaveHighScore();
    }
}
