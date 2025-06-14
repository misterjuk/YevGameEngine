#include "PlayerState.h"
#include "Player.h"
#include "GameObject.h"
#include "GridMovementComponent.h"
#include "Map.h"
#include "RenderComponent.h"
#include "Time.h"
#include <iostream>
#include "Enemy.h"

// CREATING STATES EACH TIME IS BAD SHOULD REDO IT

std::unique_ptr<PlayerState> PlayerState::HandleMovementInput(Player*, GridMovementComponent::MovementDirection)
{
    return nullptr; 
}

std::unique_ptr<PlayerState> PlayerState::HandleDigInput(Player*)
{
    return nullptr;
}

std::unique_ptr<PlayerState> PlayerState::HandleAttackInput(Player*)
{
    return nullptr; 
}

std::unique_ptr<PlayerState> PlayerState::HandleDamaged(Player*)
{
    return nullptr; 
}

std::unique_ptr<PlayerState> PlayerState::HandleStateExpired(Player*)
{
    return nullptr;
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

void PlayerIdleState::Enter(Player* )
{
   
}

void PlayerIdleState::Update(Player* , float deltaTime)
{
    m_IdleTime += deltaTime;
    

}

void PlayerIdleState::Exit(Player*)
{
   
}

std::unique_ptr<PlayerState> PlayerIdleState::HandleMovementInput(Player* player, GridMovementComponent::MovementDirection direction)
{
    if (direction != GridMovementComponent::MovementDirection::None && CanMove(player, direction))
    {
        // CREATING STATES EACH TIME IS BAD SHOULD REDO IT
        return std::make_unique<PlayerMovingState>(direction);
    }
    return nullptr;
}

std::unique_ptr<PlayerState> PlayerIdleState::HandleDigInput(Player*)
{
    // CREATING STATES EACH TIME IS BAD SHOULD REDO IT
    return std::make_unique<PlayerDiggingState>();
}

std::unique_ptr<PlayerState> PlayerIdleState::HandleAttackInput(Player*)
{
    // CREATING STATES EACH TIME IS BAD SHOULD REDO IT
    return std::make_unique<PlayerAttackingState>();
}


PlayerMovingState::PlayerMovingState(GridMovementComponent::MovementDirection direction)
    : m_Direction(direction)
{
}

void PlayerMovingState::Enter(Player* player)
{
    m_HasMoved = false;
    
 
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
   
    if (m_HasMoved)
    {

        auto newState = HandleStateExpired(player);
        if (newState)
            player->ChangeState(std::move(newState));
    }
}

void PlayerMovingState::Exit(Player*)
{
   
}

std::unique_ptr<PlayerState> PlayerMovingState::HandleMovementInput(Player* player, GridMovementComponent::MovementDirection direction)
{
    
    if (direction != m_Direction && direction != GridMovementComponent::MovementDirection::None && CanMove(player, direction))
    {
        // CREATING STATES EACH TIME IS BAD SHOULD REDO IT
        return std::make_unique<PlayerMovingState>(direction);
    }
    return nullptr;
}

std::unique_ptr<PlayerState> PlayerMovingState::HandleDigInput(Player*)
{
    // CREATING STATES EACH TIME IS BAD SHOULD REDO IT
    return std::make_unique<PlayerDiggingState>();
}

std::unique_ptr<PlayerState> PlayerMovingState::HandleAttackInput(Player*)
{
    // CREATING STATES EACH TIME IS BAD SHOULD REDO IT
    return std::make_unique<PlayerAttackingState>();
}

std::unique_ptr<PlayerState> PlayerMovingState::HandleStateExpired(Player*)
{
    // CREATING STATES EACH TIME IS BAD SHOULD REDO IT
    return std::make_unique<PlayerIdleState>();
}


void PlayerDiggingState::Enter(Player* player)
{
    m_DigTimer = 0.0f;
    
   
   
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
    
   
    if (m_DigTimer >= m_DigDuration)
    {
        auto newState = HandleStateExpired(player);
        if (newState)
            player->ChangeState(std::move(newState));
    }
}

void PlayerDiggingState::Exit(Player*)
{
    
}

std::unique_ptr<PlayerState> PlayerDiggingState::HandleStateExpired(Player*)
{
    return std::make_unique<PlayerIdleState>();
}

void PlayerAttackingState::Enter(Player* player)
{
    m_AttackTimer = 0.0f;
    m_HasHitEnemy = false;
    
   
    FindAndAttackEnemyInFront(player);
}

void PlayerAttackingState::Update(Player* player, float deltaTime)
{
    
    if (m_HasHitEnemy && m_TargetEnemy)
    {
        m_AttackTimer += deltaTime;
        
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
    // CREATING STATES EACH TIME IS BAD SHOULD REDO IT
    return std::make_unique<PlayerIdleState>();
}

void PlayerAttackingState::FindAndAttackEnemyInFront(Player* player)
{
    if (!player)
        return;

    auto movement = player->GetMovement();
    if (!movement)
        return;

   
    Position playerPos = movement->GetGridPosition();
    GridMovementComponent::MovementDirection playerDir = movement->GetDirection();


    if (playerDir == GridMovementComponent::MovementDirection::None)
        return;

    Map* map = player->GetMap();
    if (!map)
        return;

   
    const std::vector<Enemy*>& enemies = map->GetAllEnemies();


    for (Enemy* enemy : enemies)
    {
        if (enemy && enemy->IsAlive())
        {
            auto enemyMovement = enemy->GetMovement();
            if (enemyMovement)
            {
                Position enemyPos = enemyMovement->GetGridPosition();

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
                       // enemy->TakeDamage(1);
                        enemy->Stun(); 

                        m_HasHitEnemy = true;
                        m_TargetEnemy = enemy; // Store reference for continuous damage

                        return; 
                    }
                }
            }
        }
    }
}


void PlayerDyingState::Enter(Player* player)
{
    m_DeathTimer = 0.0f;
    m_AnimationComplete = false;
    
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
    
}