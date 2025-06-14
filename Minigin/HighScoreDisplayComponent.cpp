#include "HighScoreDisplayComponent.h"
#include "TextComponent.h"
#include "GameEvents.h"
#include "GameObject.h"
#include "ScoreComponent.h"
#include "GameManager.h"
#include <fstream>
#include <iostream>


HighScoreDisplayComponent::HighScoreDisplayComponent(yev::GameObject* ownerObjectPtr, yev::TextComponent* textComponent)
	: yev::Component(ownerObjectPtr), m_ScoreText{ textComponent }
{
	
    LoadHighScoreFromFile();
    //BAD WORKAROUND
	UpdateScoreDiplay(GameManager::GetInstance().GetPlayerScore());
}

HighScoreDisplayComponent::~HighScoreDisplayComponent()
{
    SaveHighScore();
}

void HighScoreDisplayComponent::UpdateScoreDiplay(int currentScore)
{
	if (m_CurrentHighScore < currentScore)
	{
		m_ScoreText->SetText("HighScore: " + std::to_string(currentScore));
		m_CurrentHighScore = currentScore;
	}
    else {
        m_ScoreText->SetText("HighScore: " + std::to_string(m_CurrentHighScore));
    }
}
//BAD WORKAROUND
void HighScoreDisplayComponent::ForceScoreUpdate()
{	
    LoadHighScoreFromFile();
	UpdateScoreDiplay(GameManager::GetInstance().GetPlayerScore());
}

void HighScoreDisplayComponent::Notify(Event event, yev::GameObject* actor)
{
	if (event == GameEvents::ScoreChanged)
	{
		if (actor)
		{
			UpdateScoreDiplay(actor->GetComponent<ScoreComponent>()->GetCurrentScore());
		}

	}
}

void HighScoreDisplayComponent::SaveHighScore()
{
    int highScore = m_CurrentHighScore;
	LoadHighScoreFromFile(); // Ensure we have the latest high score loaded
    if(highScore > m_CurrentHighScore)
    {
        m_CurrentHighScore = highScore;
        SaveHighScoreToFile();
	}
}

void HighScoreDisplayComponent::LoadHighScoreFromFile()
{
    try
    {
        std::ifstream file(m_HighScoreFilePath);
        if (file.is_open())
        {
            // Read the high score as an integer
            file >> m_CurrentHighScore;
            file.close();

            std::cout << "High score loaded: " << m_CurrentHighScore << std::endl;
        }
        else
        {
            // If file doesn't exist or can't be opened, start with 0 as high score
            m_CurrentHighScore = 0;
            std::cout << "Could not open high score file. Starting with 0." << std::endl;

            // Create the file with a default value of 0
            SaveHighScoreToFile();
        }
    }
    catch (const std::exception& e)
    {
        // Handle any exceptions that might occur during file reading
        std::cerr << "Error loading high score: " << e.what() << std::endl;
        m_CurrentHighScore = 0;
    }
}

void HighScoreDisplayComponent::SaveHighScoreToFile()
{
    try
    {
        // Open file in truncation mode to clear existing content
        std::ofstream file(m_HighScoreFilePath, std::ios::trunc);
        if (file.is_open())
        {
            // Write the new high score
            file << m_CurrentHighScore;
            file.close();

            std::cout << "High score saved: " << m_CurrentHighScore << std::endl;
        }
        else
        {
            std::cerr << "Could not open high score file for writing." << std::endl;
        }
    }
    catch (const std::exception& e)
    {
        // Handle any exceptions that might occur during file writing
        std::cerr << "Error saving high score: " << e.what() << std::endl;
    }
}