#pragma once
#include "Component.h"
#include "Map.h"
#include <memory>

namespace yev
{
    class TransformComponent;
    
    class GridMovementComponent final : public Component
    {
    public:
        GridMovementComponent(GameObject* ownerObjectPtr, Map* map);
        ~GridMovementComponent() = default;

        void Update() override;
        
        // Grid positioning
        void SetGridPosition(int x, int y);
        Position GetGridPosition() const { return m_GridPosition; }
        
        // Movement methods
        bool MoveUp();
        bool MoveDown();
        bool MoveLeft();
        bool MoveRight();
        
        // Utility methods
        void DigAtCurrentPosition();
        bool CanMoveToPosition(int x, int y) const;
        
        // Visual interpolation
        void SetInterpolationSpeed(float speed) { m_InterpolationSpeed = speed; }
        float GetInterpolationSpeed() const { return m_InterpolationSpeed; }
        
    private:
        // Update visual position based on grid position
        void UpdateVisualPosition();
        
        Position m_GridPosition{0, 0};
        Map* m_Map{nullptr};
        TransformComponent* m_Transform{nullptr};
        float m_InterpolationSpeed{10.0f}; // Controls how fast the visual position catches up to grid position
        bool m_IsMoving{false};
        glm::vec3 m_TargetPosition{0, 0, 0};
    };
}