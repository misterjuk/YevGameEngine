#ifndef PLAYERCOMMANDS_H
#define PLAYERCOMMANDS_H

#include "Command.h"
#include "GameObject.h"
#include "TransformComponent.h"
#include "Time.h"
#include "Subject.h"
#include "GameEvents.h"
#include "Player.h"
#include "GridMovementComponent.h"


    class ApplyDamageCommand : public yev::Command, public yev::Subject
    {
    public:
        ApplyDamageCommand(yev::GameObject* character) : m_Character(character) {}

        void Execute() override
        {
			NotifyObservers(GameEvents::PlayerDamaged, m_Character);
        }

    private:
        yev::GameObject* m_Character;
    };

    class ApplyScoreCommand : public yev::Command, public yev::Subject
    {
    public:
        ApplyScoreCommand(yev::GameObject* character) : m_Character(character) {}

        void Execute() override
        {
            NotifyObservers(GameEvents::PlayerScored, m_Character);
        }

    private:
        yev::GameObject * m_Character;
    };

    // New commands for Player component

    class PlayerMoveCommand : public yev::Command
    {
    public:
        PlayerMoveCommand(yev::GameObject* character, GridMovementComponent::MovementDirection direction)
            : m_Character(character), m_Direction(direction) {
        }
        
        void Execute() override
        {
            if (m_Character && m_Character->HasComponent<Player>())
            {
                m_Character->GetComponent<Player>()->HandleMovementInput(m_Direction);
            }
        }
        
    private:
        yev::GameObject* m_Character;
        GridMovementComponent::MovementDirection m_Direction;
    };

    class PlayerMoveUpCommand : public PlayerMoveCommand
    {
    public:
        PlayerMoveUpCommand(yev::GameObject* character)
            : PlayerMoveCommand(character, GridMovementComponent::MovementDirection::Up) {
        }
    };

    class PlayerMoveDownCommand : public PlayerMoveCommand
    {
    public:
        PlayerMoveDownCommand(yev::GameObject* character)
            : PlayerMoveCommand(character, GridMovementComponent::MovementDirection::Down) {
        }
    };

    class PlayerMoveLeftCommand : public PlayerMoveCommand
    {
    public:
        PlayerMoveLeftCommand(yev::GameObject* character)
            : PlayerMoveCommand(character, GridMovementComponent::MovementDirection::Left) {
        }
    };

    class PlayerMoveRightCommand : public PlayerMoveCommand
    {
    public:
        PlayerMoveRightCommand(yev::GameObject* character)
            : PlayerMoveCommand(character, GridMovementComponent::MovementDirection::Right) {
        }
    };

   

    class PlayerAttackCommand : public yev::Command
    {
    public:
        PlayerAttackCommand(yev::GameObject* character)
            : m_Character(character) {
        }
        
        void Execute() override
        {
            if (m_Character && m_Character->HasComponent<Player>())
            {
                m_Character->GetComponent<Player>()->HandleAttackInput();
            }
        }
        
    private:
        yev::GameObject* m_Character;
    };
 
#endif 