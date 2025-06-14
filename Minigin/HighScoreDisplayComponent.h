#ifndef HIGHSCOREDISPLAYCOMPONENT_H
#define HIGHSCOREDISPLAYCOMPONENT_H


#include "Component.h"
#include <string>
#include "IObserver.h"

namespace yev
{
    class GameObject;
    class TextComponent;
}

//maybe inherit from textcomponent
class HighScoreDisplayComponent final : public yev::Component, public yev::IObserver
{
public:
    HighScoreDisplayComponent(yev::GameObject* ownerObjectPtr, yev::TextComponent* textComponent);
    ~HighScoreDisplayComponent();

    HighScoreDisplayComponent(const HighScoreDisplayComponent&) = delete;
    HighScoreDisplayComponent(HighScoreDisplayComponent&&) = delete;
    HighScoreDisplayComponent& operator=(const HighScoreDisplayComponent&) = delete;
    HighScoreDisplayComponent& operator=(HighScoreDisplayComponent&&) = delete;

    void UpdateScoreDiplay(int currentHighScore);
    void ForceScoreUpdate();

    void Notify(Event event, yev::GameObject* actor) override;

	void SaveHighScore();
private:

    void LoadHighScoreFromFile();
    void SaveHighScoreToFile();
    const std::string m_HighScoreFilePath = "Data/highscore.txt";


	int m_CurrentHighScore{ 0 };
    yev::TextComponent* m_ScoreText{ nullptr };


};



#endif 


