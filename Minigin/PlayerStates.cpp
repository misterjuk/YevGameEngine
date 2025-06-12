#include "PlayerState.h"
#include "Player.h"
#include "GameObject.h"
#include "GridMovementComponent.h"
#include "Map.h"
#include "RenderComponent.h"
#include "Time.h"
#include <iostream>

// Base PlayerState methods
std::unique_ptr<PlayerState> PlayerState::HandleMovementInput(Player*, GridMovementComponent::MovementDirection)
{
    return nullptr; // Default: no state change
}

std::unique_ptr<PlayerState> PlayerState::HandleDigInput(Player*)
{
    return nullptr; // Default: no state change
}

std::unique_ptr<PlayerState> PlayerState::HandleAttackInput(Player*)
{
    return nullptr; // Default: no state change
}

std::unique_ptr<PlayerState> PlayerState::HandleDamaged(Player*)
{
    return nullptr; // Default: no state change
}

std::unique_ptr<PlayerState> PlayerState::HandleStateExpired(Player*)
{
    return nullptr; // Default: no state change
}

bool PlayerState::CanMove(Player* player, GridMovementComponent::MovementDirection direction) const
{
    if (!player)
        return false;
    
    auto movement = player->GetMovement();
    if (!movement)
        return false;
    
    Position pos = movement->GetGridPosition();
    int newX = pos.x;
    int newY = pos.y;
    
    switch (direction)
    {
    case GridMovementComponent::MovementDirection::Up:
        newY--; break;
    case GridMovementComponent::MovementDirection::Down:
        newY++; break;
    case GridMovementComponent::MovementDirection::Left:
        newX--; break;
    case GridMovementComponent::MovementDirection::Right:
        newX++; break;
    default:
        return false;
    }
    
    return movement->CanMoveToPosition(newX, newY);
}

// PlayerIdleState implementation
void PlayerIdleState::Enter(Player* player)
{
    m_IdleTime = 0.0f;
    
    // Visual indicator for idle state (could change animation/texture)
    if (auto owner = player->GetOwner())
    {
        if (auto render = owner->GetComponent<yev::RenderComponent>())
        {
            // Change appearance for idle state
            // render->SetTexture("PlayerIdle.png");
        }
    }
}

void PlayerIdleState::Update(Player* , float deltaTime)
{
    m_IdleTime += deltaTime;
    
    // Idle animations or logic could go here
}

void PlayerIdleState::Exit(Player*)
{
    // Clean up idle state resources if needed
}

std::unique_ptr<PlayerState> PlayerIdleState::HandleMovementInput(Player* player, GridMovementComponent::MovementDirection direction)
{
    if (direction != GridMovementComponent::MovementDirection::None && CanMove(player, direction))
    {
        return std::make_unique<PlayerMovingState>(direction);
    }
    return nullptr;
}

std::unique_ptr<PlayerState> PlayerIdleState::HandleDigInput(Player*)
{
    return std::make_unique<PlayerDiggingState>();
}

std::unique_ptr<PlayerState> PlayerIdleState::HandleAttackInput(Player*)
{
    return std::make_unique<PlayerAttackingState>();
}

// PlayerMovingState implementation
PlayerMovingState::PlayerMovingState(GridMovementComponent::MovementDirection direction)
    : m_Direction(direction)
{
}

void PlayerMovingState::Enter(Player* player)
{
    m_HasMoved = false;
    
    // Visual indicator for moving state (could change animation/texture)
    if (auto owner = player->GetOwner())
    {
        playerRenderComponent = owner->GetComponent<yev::RenderComponent>();
        
    }
    
    // Perform the actual movement
    auto movement = player->GetMovement();
    if (!movement)
        return;
    
    switch (m_Direction)
    {
    case GridMovementComponent::MovementDirection::Up:

        playerRenderComponent->SetTexture(m_RunUp1);
        m_HasMoved = movement->MoveUp();
        break;
    case GridMovementComponent::MovementDirection::Down:
        playerRenderComponent->SetTexture(m_RunDown1);
        m_HasMoved = movement->MoveDown();
        break;
    case GridMovementComponent::MovementDirection::Left:
        playerRenderComponent->SetTexture(m_RunLeft1);
        m_HasMoved = movement->MoveLeft();
        break;
    case GridMovementComponent::MovementDirection::Right:
        playerRenderComponent->SetTexture(m_RunRight1);
        m_HasMoved = movement->MoveRight();
        break;
    default:
        break;
    }
}

void PlayerMovingState::Update(Player* player, float )
{
    // Automatically transition back to idle once movement is complete
    if (m_HasMoved)
    {

        auto newState = HandleStateExpired(player);
        if (newState)
            player->ChangeState(std::move(newState));
    }
}

void PlayerMovingState::Exit(Player*)
{
    // Clean up moving state resources if needed
}

std::unique_ptr<PlayerState> PlayerMovingState::HandleMovementInput(Player* player, GridMovementComponent::MovementDirection direction)
{
    // If we get a new movement input, create a new moving state in that direction
    if (direction != m_Direction && direction != GridMovementComponent::MovementDirection::None && CanMove(player, direction))
    {
        return std::make_unique<PlayerMovingState>(direction);
    }
    return nullptr;
}

std::unique_ptr<PlayerState> PlayerMovingState::HandleDigInput(Player*)
{
    return std::make_unique<PlayerDiggingState>();
}

std::unique_ptr<PlayerState> PlayerMovingState::HandleAttackInput(Player*)
{
    return std::make_unique<PlayerAttackingState>();
}

std::unique_ptr<PlayerState> PlayerMovingState::HandleStateExpired(Player*)
{
    return std::make_unique<PlayerIdleState>();
}

// PlayerDiggingState implementation
void PlayerDiggingState::Enter(Player* player)
{
    m_DigTimer = 0.0f;
    
    // Visual indicator for digging state
    if (auto owner = player->GetOwner())
    {
        if (auto render = owner->GetComponent<yev::RenderComponent>())
        {
            // Change appearance for digging state
            // render->SetTexture("PlayerDigging.png");
        }
    }
    
    // Perform the dig action
    auto movement = player->GetMovement();
    if (movement)
    {
        movement->DigAtCurrentPosition();
    }
}

void PlayerDiggingState::Update(Player* player, float deltaTime)
{
    m_DigTimer += deltaTime;
    
    // Return to idle after digging completes
    if (m_DigTimer >= m_DigDuration)
    {
        auto newState = HandleStateExpired(player);
        if (newState)
            player->ChangeState(std::move(newState));
    }
}

void PlayerDiggingState::Exit(Player*)
{
    // Clean up digging state resources if needed
}

std::unique_ptr<PlayerState> PlayerDiggingState::HandleStateExpired(Player*)
{
    return std::make_unique<PlayerIdleState>();
}

// PlayerAttackingState implementation
void PlayerAttackingState::Enter(Player* player)
{
    m_AttackTimer = 0.0f;
    
    // Visual indicator for attack state
    if (auto owner = player->GetOwner())
    {
        if (auto render = owner->GetComponent<yev::RenderComponent>())
        {
            // Change appearance for attack state
            // render->SetTexture("PlayerAttacking.png");
        }
    }
    
    // In a complete implementation, this would create a pump attack object
    // or check for enemies in front to damage
}

void PlayerAttackingState::Update(Player* player, float deltaTime)
{
    m_AttackTimer += deltaTime;
    
    // Return to idle after attack completes
    if (m_AttackTimer >= m_AttackDuration)
    {
        auto newState = HandleStateExpired(player);
        if (newState)
            player->ChangeState(std::move(newState));
    }
}

void PlayerAttackingState::Exit(Player*)
{
    // Clean up attack state resources if needed
}

std::unique_ptr<PlayerState> PlayerAttackingState::HandleStateExpired(Player*)
{
    return std::make_unique<PlayerIdleState>();
}

// PlayerDyingState implementation
void PlayerDyingState::Enter(Player* player)
{
    m_DeathTimer = 0.0f;
    m_AnimationComplete = false;
    
    // Visual indicator for death state
    if (auto owner = player->GetOwner())
    {
        if (auto render = owner->GetComponent<yev::RenderComponent>())
        {
            // Change appearance for death state
            // render->SetTexture("PlayerDying.png");
        }
    }
}

void PlayerDyingState::Update(Player*, float deltaTime)
{
    if (m_AnimationComplete)
        return;
    
    m_DeathTimer += deltaTime;
    
    // Complete death animation
    if (m_DeathTimer >= m_DeathAnimationDuration)
    {
        m_AnimationComplete = true;
        
        // In a real game, this would trigger a game over or respawn sequence
    }
}

void PlayerDyingState::Exit(Player*)
{
    // Clean up death state resources if needed
}