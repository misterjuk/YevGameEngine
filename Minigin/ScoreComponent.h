#ifndef SCORECOMPONENT_H
#define SCORECOMPONENT_H


#include "Component.h"
#include <string>
#include "IObserver.h"
#include "Subject.h"


namespace yev
{
    class GameObject;
    class TextComponent;
}

//maybe inherit from textcomponent
class ScoreComponent final : public yev::Component, public yev::IObserver, public yev::Subject
{
public:
    ScoreComponent(yev::GameObject* ownerObjectPtr);
    ~ScoreComponent() = default;

    ScoreComponent(const ScoreComponent&) = delete;
    ScoreComponent(ScoreComponent&&) = delete;
    ScoreComponent& operator=(const ScoreComponent&) = delete;
    ScoreComponent& operator=(ScoreComponent&&) = delete;

 
    int GetCurrentScore() const { return m_currentScore; }
    void AddScore(int score);

    void Notify(Event event, yev::GameObject* actor) override;

    // Add to ScoreComponent.h
    void SetScore(int score);

private:

    int m_currentScore;

};



#endif // !SCORECOMPONENT_H


