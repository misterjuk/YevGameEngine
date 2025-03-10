#ifndef SCORECOMPONENT_H
#define SCORECOMPONENT_H


#include "Component.h"
#include <string>
#include "IObserver.h"

namespace yev
{
    class GameObject;
    class TextComponent;
}

//maybe inherit from textcomponent
class ScoreComponent final : public yev::Component, public yev::IObserver
{
public:
    ScoreComponent(yev::GameObject* ownerObjectPtr, yev::TextComponent* textComponent);
    ~ScoreComponent() = default;

    ScoreComponent(const ScoreComponent&) = delete;
    ScoreComponent(ScoreComponent&&) = delete;
    ScoreComponent& operator=(const ScoreComponent&) = delete;
    ScoreComponent& operator=(ScoreComponent&&) = delete;

 
    int GetCurrentScore() const { return m_currentScore; }
    void AddScore(int score);

    // Method to save the high score to a file
    void SaveHighScore();

    // Method to load the high score from a file
    void LoadHighScore();

    void Notify(Event event, yev::GameObject* actor) override;

private:
    // Path to the high score file
    const std::string m_highScoreFilePath = "Data/highscore.txt";

    // Current score of the player
    int m_currentScore;

    // High score loaded from the file
    int m_highScore;

    yev::TextComponent* m_ScoreText{ nullptr };
    // Method to update high score if the current score is higher
    void UpdateHighScore();
};



#endif // !SCORECOMPONENT_H


