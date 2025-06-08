#include "GridMovementComponent.h"
#include "GameObject.h"
#include "TransformComponent.h"
#include "Time.h"


    GridMovementComponent::GridMovementComponent(yev::GameObject* ownerObjectPtr, Map* map)
        : yev::Component(ownerObjectPtr), m_Map(map)
    {
        m_Transform = ownerObjectPtr->GetComponent<yev::TransformComponent>();
        
        // Initialize to player spawn position if available
        if (m_Map)
        {
            m_GridPosition = m_Map->GetPlayerSpawnPosition();
            UpdateVisualPosition();
        }
    }

    void GridMovementComponent::Update()
    {
        if (!m_Transform || !m_Map) return;
        
        // If character is moving, interpolate to target position
        if (m_IsMoving)
        {
            glm::vec3 currentPos = m_Transform->GetLocalPosition();
            float deltaTime = yev::Time::GetInstance().GetDeltaTime();
            
            // Calculate new position with smooth interpolation
            glm::vec3 newPos = glm::mix(currentPos, m_TargetPosition, deltaTime * m_InterpolationSpeed);
            m_Transform->SetLocalPosition(newPos.x, newPos.y, newPos.z);
            
            // Check if we've reached the target (or close enough)
            float distance = glm::distance(newPos, m_TargetPosition);
            if (distance < 1.0f)
            {
                m_Transform->SetLocalPosition(m_TargetPosition.x, m_TargetPosition.y, m_TargetPosition.z);
                m_IsMoving = false;
                
                // Auto-dig when moving to an earth tile
                DigAtCurrentPosition();
            }
        }
    }
    
    void GridMovementComponent::SetGridPosition(int x, int y)
    {
        if (m_Map->IsValidPosition(x, y))
        {
            m_GridPosition = {x, y};
            UpdateVisualPosition();
        }
    }
    
    bool GridMovementComponent::MoveUp()
    {
        if (CanMoveToPosition(m_GridPosition.x, m_GridPosition.y - 1))
        {
            m_GridPosition.y -= 1;
            UpdateVisualPosition();
            return true;
        }
        return false;
    }
    
    bool GridMovementComponent::MoveDown()
    {
        if (CanMoveToPosition(m_GridPosition.x, m_GridPosition.y + 1))
        {
            m_GridPosition.y += 1;
            UpdateVisualPosition();
            return true;
        }
        return false;
    }
    
    bool GridMovementComponent::MoveLeft()
    {
        if (CanMoveToPosition(m_GridPosition.x - 1, m_GridPosition.y))
        {
            m_GridPosition.x -= 1;
            UpdateVisualPosition();
            return true;
        }
        return false;
    }
    
    bool GridMovementComponent::MoveRight()
    {
        if (CanMoveToPosition(m_GridPosition.x + 1, m_GridPosition.y))
        {
            m_GridPosition.x += 1;
            UpdateVisualPosition();
            return true;
        }
        return false;
    }
    
    void GridMovementComponent::DigAtCurrentPosition()
    {
        if (m_Map)
        {
            m_Map->DigTunnel(m_GridPosition.x, m_GridPosition.y);
        }
    }
    
    bool GridMovementComponent::CanMoveToPosition(int x, int y) const
    {
        if (!m_Map || !m_Map->IsValidPosition(x, y))
            return false;
            
        TileType tileType = m_Map->GetTileAt(x, y);
        
        // Can move to empty spaces, earth tiles, and spawns
        return tileType != TileType::Rock;
    }
    
    void GridMovementComponent::UpdateVisualPosition()
    {
        if (!m_Transform || !m_Map) return;

        // Calculate pixel position from grid position
        int tileSize = m_Map->GetTileSize();
        float x = static_cast<float>(m_GridPosition.x * tileSize);
        float y = static_cast<float>(m_GridPosition.y * tileSize);

        // Store as target position and set the character to moving state
        m_TargetPosition = { x, y, m_Transform->GetLocalPosition().z };
        m_IsMoving = true;
    }