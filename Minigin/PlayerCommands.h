#ifndef PLAYERCOMMANDS_H
#define PLAYERCOMMANDS_H




#include "Command.h"
#include "GameObject.h"
#include "TransformComponent.h"
#include "Time.h"

namespace yev
{
    class MoveCommand : public Command
    {
    public:
        MoveCommand(GameObject* character, float speed, const glm::vec3& direction)
            : m_Character(character), m_Speed{ speed }, m_Direction(direction) {
        }

        void Execute() override
        {
            if (m_Character && m_Character->HasComponent<TransformComponent>())
            {
                float deltaTime = Time::GetInstance().GetDeltaTime();
                m_Character->GetComponent<TransformComponent>()->Translate(m_Direction * m_Speed * deltaTime);
            }
        }

        void SetSpeed(float speed) { m_Speed = speed; };

    private:
        GameObject* m_Character;  
        glm::vec3 m_Direction;
        float m_Speed = 200.0f;
    };

    class MoveUpCommand : public MoveCommand
    {
    public:
        MoveUpCommand(GameObject* character, float speed)
            : MoveCommand(character,speed,glm::vec3(0.0f, -1.0f, 0.0f)) {
        }
    };

    class MoveDownCommand : public MoveCommand
    {
    public:
        MoveDownCommand(GameObject* character, float speed)
            : MoveCommand(character,speed,glm::vec3(0.0f, 1.0f, 0.0f)) {
        }
    };

    class MoveLeftCommand : public MoveCommand
    {
    public:
        MoveLeftCommand(GameObject* character, float speed)
            : MoveCommand(character,speed,glm::vec3(-1.0f, 0.0f, 0.0f)) {
        }
    };

    class MoveRightCommand : public MoveCommand
    {
    public:
        MoveRightCommand(GameObject* character, float speed)
            : MoveCommand(character,speed,glm::vec3(1.0f, 0.0f, 0.0f)) {
        }
    };
}
 
#endif 