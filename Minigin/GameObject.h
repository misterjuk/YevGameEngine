#pragma once
#include <memory>
#include <vector>
#include <stdexcept>
#include "Component.h"


namespace yev
{
	class Texture2D;

	class GameObject final
	{
	public:
		virtual void Update();
		virtual void Render() const;

		GameObject() = default;
		~GameObject();
		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;


        void MarkForDeletion() { m_markedForDeletion = true; }
        bool IsMarkedForDeletion() const { return m_markedForDeletion; }


        template <typename T, typename... Args>
        T* AddComponent(Args&&... args)
        {
            static_assert(std::is_base_of_v<Component, T>, "T must derive from Component");

            try
            {
                auto& componentPtr = m_Components.emplace_back(std::make_unique<T>(std::forward<Args>(args)...));
                return static_cast<T*>(componentPtr.get());
            }
            catch (const std::exception& e)
            {
                throw std::runtime_error("AddComponent failed: " + std::string(e.what()));
            }
        }

        template <typename T>
        void RemoveComponent()
        {
            try
            {
                bool found = false;

                for (auto& component : m_Components)
                {
                    if (dynamic_cast<T*>(component.get()) != nullptr)
                    {
                        component->MarkForDeletion();
                        found = true;
                    }
                }

                // If no component of type T was found, throw an exception
                if (!found)
                {
                    throw std::runtime_error("RemoveComponent failed: Component of requested type not found.");
                }
            }
            catch (const std::exception& e)
            {
                // Handle the exception: log or rethrow with additional context
                throw std::runtime_error("RemoveComponent encountered an error: " + std::string(e.what()));
            }
        }

        template <typename T>
        T* GetComponent() const
        {
            try
            {
                for (const auto& component : m_Components)
                {
                    if (auto castedComponent = dynamic_cast<T*>(component.get()))
                        return castedComponent;
                }
                return nullptr;
            }
            catch (const std::exception& e)
            {
                // Handle exception: log the error, rethrow with a custom message, or take appropriate action.
                throw std::runtime_error("GetComponent failed: " + std::string(e.what()));
            }
        }

        template <typename T>
        bool HasComponent() const
        {
            try
            {
                for (const auto& component : m_Components)
                {
                    if (dynamic_cast<T*>(component.get()) != nullptr)
                        return true;
                }
                return false;
            }
            catch (const std::exception& e)
            {
                // Handle exception: log the error, rethrow with a custom message, or take appropriate action.
                throw std::runtime_error("HasComponent failed: " + std::string(e.what()));
            }
        }


        void SetParent(GameObject* parent, bool keepWorldPosition);

        GameObject* GetParent() const { return m_Parent; };

	private:
	
        std::vector<std::unique_ptr<Component>> m_Components{};

        bool m_markedForDeletion{ false };


        GameObject* m_Parent{};
        std::vector<GameObject*> m_Children{};

        bool IsChild(GameObject* childToCheck) const;
        void AddChild(GameObject* child);
        void RemoveChild(GameObject* child);
        void SetPositionDirty(const bool isPositionDirty);
	};
}


