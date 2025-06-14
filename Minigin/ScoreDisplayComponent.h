#ifndef SCOREDISPLAYCOMPONENT_H
#define SCOREDISPLAYCOMPONENT_H


#include "Component.h"
#include <string>
#include "IObserver.h"

namespace yev
{
    class GameObject;
    class TextComponent;
}

//maybe inherit from textcomponent
class ScoreDisplayComponent final : public yev::Component, public yev::IObserver
{
public:
    ScoreDisplayComponent(yev::GameObject* ownerObjectPtr, yev::TextComponent* textComponent);
    ~ScoreDisplayComponent() = default;

    ScoreDisplayComponent(const ScoreDisplayComponent&) = delete;
    ScoreDisplayComponent(ScoreDisplayComponent&&) = delete;
    ScoreDisplayComponent& operator=(const ScoreDisplayComponent&) = delete;
    ScoreDisplayComponent& operator=(ScoreDisplayComponent&&) = delete;

    void UpdateScoreDiplay(int currentHealth);
    void ForceScoreUpdate();

    void Notify(Event event, yev::GameObject* actor) override;

private:


    yev::TextComponent* m_ScoreText{ nullptr };


};



#endif 


