#pragma once
#include "Command.h"
#include "GameObject.h"
#include "GridMovementComponent.h"
#include "GameEvents.h"
#include "Subject.h"
#include "Time.h"

class GridMoveCommand : public yev::Command
{
public:
    GridMoveCommand(yev::GameObject* character)
        : m_Character(character) {
    }
    
    virtual ~GridMoveCommand() = default;
    
protected:
    yev::GameObject* m_Character;
};

class GridMoveUpCommand : public GridMoveCommand
{
public:
    GridMoveUpCommand(yev::GameObject* character)
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
    GridMoveDownCommand(yev::GameObject* character)
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
    GridMoveLeftCommand(yev::GameObject* character)
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
    GridMoveRightCommand(yev::GameObject* character)
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

