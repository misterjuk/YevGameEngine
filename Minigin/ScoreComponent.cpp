#include "ScoreComponent.h"
#include <fstream>
#include <iostream>
#include "GameObject.h"
#include "TextComponent.h"
#include "GameEvents.h"

ScoreComponent::ScoreComponent(yev::GameObject* ownerObjectPtr, yev::TextComponent* textComponent)
    : yev::Component(ownerObjectPtr), m_currentScore(0), m_highScore(0), m_ScoreText{ textComponent }
{

    //LoadHighScore();

}

void ScoreComponent::AddScore(int score)
{

    m_currentScore += score;
	m_ScoreText->SetText("Score: " + std::to_string(m_currentScore));
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

        m_ScoreText->SetText("HighScore: " + std::to_string(m_highScore));
        file.close();
    }
    else
    {
        std::cerr << "Unable to open file for loading high score: " << m_highScoreFilePath << std::endl;
    }
}

void ScoreComponent::Notify(Event event, yev::GameObject*)
{
	if (event == GameEvents::PlayerScored)
	AddScore(10);
}

void ScoreComponent::UpdateHighScore()
{
    if (m_currentScore > m_highScore)
    {
        m_highScore = m_currentScore;
        SaveHighScore();
    }
}
