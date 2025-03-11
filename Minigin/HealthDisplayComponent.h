#ifndef HEALTHDISPLAYCOMPONENT_H
#define HEALTHDISPLAYCOMPONENT_H


#include "Component.h"
#include <string>
#include "IObserver.h"

namespace yev
{
    class GameObject;
    class TextComponent;
}

//maybe inherit from textcomponent
class HealthDisplayComponent final : public yev::Component, public yev::IObserver
{
public:
    HealthDisplayComponent(yev::GameObject* ownerObjectPtr, yev::TextComponent* textComponent);
    ~HealthDisplayComponent() = default;

    HealthDisplayComponent(const HealthDisplayComponent&) = delete;
    HealthDisplayComponent(HealthDisplayComponent&&) = delete;
    HealthDisplayComponent& operator=(const HealthDisplayComponent&) = delete;
    HealthDisplayComponent& operator=(HealthDisplayComponent&&) = delete;

    void UpdateHealthDiplay(int currentHealth);

    void Notify(Event event, yev::GameObject* actor) override;

private:


    yev::TextComponent* m_HealthText{ nullptr };

   
};



#endif 


