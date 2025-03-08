#ifndef PLAYERCOMMANDS_H
#define PLAYERCOMMANDS_H




#include "Command.h"
#include "GameObject.h"
#include "TransformComponent.h"
#include "Time.h"

namespace dae
{
    class MoveUpCommand : public Command
    {
    public:
        MoveUpCommand(GameObject* character) : m_Character(character) {}

        void Execute() override
        {

            if (m_Character->HasComponent<TransformComponent>())
            {
                m_Character->GetComponent<TransformComponent>()->Translate(glm::vec3(0.0f,-200.0f * Time::GetInstance().GetDeltaTime(), 0.0f));
            }
        }

    private:
        GameObject* m_Character;
    };

    class MoveDownCommand : public Command
    {
    public:
        MoveDownCommand(GameObject* character) : m_Character(character) {}

        void Execute() override
        {

            if (m_Character->HasComponent<TransformComponent>())
            {
                m_Character->GetComponent<TransformComponent>()->Translate(glm::vec3(0.0f, 200.0f * Time::GetInstance().GetDeltaTime(), 0.0f));
            }
        }

    private:
        GameObject* m_Character;
    };

    class MoveLeftCommand : public Command
    {
    public:
        MoveLeftCommand(GameObject* character) : m_Character(character) {}

        void Execute() override
        {

            if (m_Character->HasComponent<TransformComponent>())
            {
                m_Character->GetComponent<TransformComponent>()->Translate(glm::vec3(-200.0f * Time::GetInstance().GetDeltaTime(), 0.0f, 0.0f));
            }
        }

    private:
        GameObject* m_Character;
    };

    class MoveRightCommand : public Command
    {
    public:
        MoveRightCommand(GameObject* character) : m_Character(character) {}

        void Execute() override
        {
            if (m_Character->HasComponent<TransformComponent>())
            {
                m_Character->GetComponent<TransformComponent>()->Translate(glm::vec3(200.0f * Time::GetInstance().GetDeltaTime(), 0.0f, 0.0f));
            }
        }

    private:
        GameObject* m_Character;
    };
}
 
#endif 