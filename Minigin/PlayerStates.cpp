#include "PlayerState.h"
#include "Player.h"
#include "GameObject.h"
#include "GridMovementComponent.h"
#include "Map.h"
#include "RenderComponent.h"
#include "Time.h"
#include <iostream>
#include "Enemy.h"

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

void PlayerAttackingState::Enter(Player* player)
{
    m_AttackTimer = 0.0f;
    m_HasHitEnemy = false;
    
    // Visual indicator for attack state
    if (auto owner = player->GetOwner())
    {
        if (auto render = owner->GetComponent<yev::RenderComponent>())
        {
            // Change appearance for attack state
            // render->SetTexture("PlayerAttacking.png");
        }
    }
    
    FindAndAttackEnemyInFront(player);
}

void PlayerAttackingState::Update(Player* player, float deltaTime)
{
    

    // Apply continuous damage to enemy if hit (simulates pumping)
    if (m_HasHitEnemy && m_TargetEnemy)
    {
        m_AttackTimer += deltaTime;
        // Apply damage every 0.3 seconds to simulate pumping action
        if (m_AttackTimer > 0.3f)
        {
            m_AttackTimer = 0.0f;
            
            bool result = m_TargetEnemy->TakeDamage(1);

           if(!result)
            {
                // If the enemy is dead, reset the target
                m_HasHitEnemy = false;
                m_TargetEnemy = nullptr;

                auto newState = HandleStateExpired(player);
                if (newState)
                    player->ChangeState(std::move(newState));
                return;
            }
            // If the player has a score component, reward player for each pump
         /*   if (auto scoreComp = player->GetOwner()->GetComponent<ScoreComponent>())
            {
                scoreComp->AddScore(10);
            }*/
        }
    }
    else {
        auto newState = HandleStateExpired(player);
        if (newState)
            player->ChangeState(std::move(newState));
        return;
    }

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
    m_TargetEnemy = nullptr;
}

std::unique_ptr<PlayerState> PlayerAttackingState::HandleStateExpired(Player*)
{
    return std::make_unique<PlayerIdleState>();
}

void PlayerAttackingState::FindAndAttackEnemyInFront(Player* player)
{
    if (!player)
        return;

    auto movement = player->GetMovement();
    if (!movement)
        return;

    // Get player's position and direction
    Position playerPos = movement->GetGridPosition();
    GridMovementComponent::MovementDirection playerDir = movement->GetDirection();

    // Skip if the player doesn't have a direction (shouldn't happen, but safety check)
    if (playerDir == GridMovementComponent::MovementDirection::None)
        return;

    Map* map = player->GetMap();
    if (!map)
        return;

    // Get all enemies from the map registry
    const std::vector<Enemy*>& enemies = map->GetAllEnemies();

    // Check if any enemies are in the attack direction
    for (Enemy* enemy : enemies)
    {
        if (enemy && enemy->IsAlive())
        {
            auto enemyMovement = enemy->GetMovement();
            if (enemyMovement)
            {
                Position enemyPos = enemyMovement->GetGridPosition();

                // Check if enemy is in the attack direction
                bool inAttackDirection = false;
                int distance = 0;

                switch (playerDir)
                {
                case GridMovementComponent::MovementDirection::Up:
                    inAttackDirection = (enemyPos.x == playerPos.x && enemyPos.y < playerPos.y);
                    distance = playerPos.y - enemyPos.y;
                    break;
                case GridMovementComponent::MovementDirection::Down:
                    inAttackDirection = (enemyPos.x == playerPos.x && enemyPos.y > playerPos.y);
                    distance = enemyPos.y - playerPos.y;
                    break;
                case GridMovementComponent::MovementDirection::Left:
                    inAttackDirection = (enemyPos.y == playerPos.y && enemyPos.x < playerPos.x);
                    distance = playerPos.x - enemyPos.x;
                    break;
                case GridMovementComponent::MovementDirection::Right:
                    inAttackDirection = (enemyPos.y == playerPos.y && enemyPos.x > playerPos.x);
                    distance = enemyPos.x - playerPos.x;
                    break;
                default:
                    break;
                }

                if (inAttackDirection && distance <= m_AttackRange)
                {
                    // Check if there are any walls between player and enemy
                    bool pathClear = true;
                    for (int i = 1; i < distance; i++)
                    {
                        Position checkPos = playerPos;
                        switch (playerDir)
                        {
                        case GridMovementComponent::MovementDirection::Up:
                            checkPos.y -= i;
                            break;
                        case GridMovementComponent::MovementDirection::Down:
                            checkPos.y += i;
                            break;
                        case GridMovementComponent::MovementDirection::Left:
                            checkPos.x -= i;
                            break;
                        case GridMovementComponent::MovementDirection::Right:
                            checkPos.x += i;
                            break;
                        default:
                            break;
                        }

                        if (map->GetTileAt(checkPos.x, checkPos.y) == TileType::Rock)
                        {
                            pathClear = false;
                            break;
                        }
                    }

                    if (pathClear)
                    {
                        // Found an enemy to attack!
                       // enemy->TakeDamage(1);
                        enemy->Stun(); // Stun the enemy immediately

                        m_HasHitEnemy = true;
                        m_TargetEnemy = enemy; // Store reference for continuous damage

                        // If the player has a score component, reward player for hit
                        /*if (auto scoreComp = player->GetOwner()->GetComponent<ScoreComponent>())
                        {
                            scoreComp->AddScore(50);
                        }*/

                        return; // Stop after hitting the first enemy
                    }
                }
            }
        }
    }
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

void PlayerDyingState::Update(Player* player, float deltaTime)
{
    if (m_AnimationComplete)
        return;
    
    m_DeathTimer += deltaTime;
    
    // Complete death animation
    if (m_DeathTimer >= m_DeathAnimationDuration)
    {
        m_AnimationComplete = true;
        
        if (auto owner = player->GetOwner())
        {
            owner->MarkForDeletion();
        }
    }
}

void PlayerDyingState::Exit(Player*)
{
    // Clean up death state resources if needed
}