#pragma once
#include <memory>
#include <string>
#include "Singleton.h"

class GameManager final : public yev::Singleton<GameManager>
{
public:
    GameManager() = default;
    GameManager(const GameManager&) = delete;
    GameManager(GameManager&&) = delete;
    GameManager& operator=(const GameManager&) = delete;
    GameManager& operator=(GameManager&&) = delete;

    int GetPlayerScore() const { return m_PlayerScore; }
    void SetPlayerScore(int score) { m_PlayerScore = score; }
    
    int GetPlayerHealth() const { return m_PlayerHealth; }
    void SetPlayerHealth(int health) { m_PlayerHealth = health; }
    
    int GetCurrentLevel() const { return m_CurrentLevel; }
    void SetCurrentLevel(int level) { m_CurrentLevel = level; }
    
    bool SkipToLevel(int levelNumber);

private:

   

    // Game state
    int m_PlayerScore{0};
    int m_PlayerHealth{3};
    int m_CurrentLevel{0};
};