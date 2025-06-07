#pragma once
#include "Command.h"
#include "GameObject.h"
#include "GridMovementComponent.h"
#include "GameEvents.h"
#include "Subject.h"

namespace yev
{
    class GridMoveCommand : public Command
    {
    public:
        GridMoveCommand(GameObject* character)
            : m_Character(character) {
        }
        
        virtual ~GridMoveCommand() = default;
        
    protected:
        GameObject* m_Character;
    };

    class GridMoveUpCommand : public GridMoveCommand
    {
    public:
        GridMoveUpCommand(GameObject* character)
            : GridMoveCommand(character) {
        }

        void Execute() override
        {
            if (m_Character && m_Character->HasComponent<GridMovementComponent>())
            {
                m_Character->GetComponent<GridMovementComponent>()->MoveUp();
            }
        }
    };

    class GridMoveDownCommand : public GridMoveCommand
    {
    public:
        GridMoveDownCommand(GameObject* character)
            : GridMoveCommand(character) {
        }

        void Execute() override
        {
            if (m_Character && m_Character->HasComponent<GridMovementComponent>())
            {
                m_Character->GetComponent<GridMovementComponent>()->MoveDown();
            }
        }
    };

    class GridMoveLeftCommand : public GridMoveCommand
    {
    public:
        GridMoveLeftCommand(GameObject* character)
            : GridMoveCommand(character) {
        }

        void Execute() override
        {
            if (m_Character && m_Character->HasComponent<GridMovementComponent>())
            {
                m_Character->GetComponent<GridMovementComponent>()->MoveLeft();
            }
        }
    };

    class GridMoveRightCommand : public GridMoveCommand
    {
    public:
        GridMoveRightCommand(GameObject* character)
            : GridMoveCommand(character) {
        }

        void Execute() override
        {
            if (m_Character && m_Character->HasComponent<GridMovementComponent>())
            {
                m_Character->GetComponent<GridMovementComponent>()->MoveRight();
            }
        }
    };

    class DigCommand : public Command
    {
    public:
        DigCommand(GameObject* character)
            : m_Character(character) {
        }

        void Execute() override
        {
            if (m_Character && m_Character->HasComponent<GridMovementComponent>())
            {
                m_Character->GetComponent<GridMovementComponent>()->DigAtCurrentPosition();
            }
        }
        
    private:
        GameObject* m_Character;
    };
}