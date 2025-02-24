#pragma once
namespace dae
{
    class GameObject;

    class Component
    {
    public:
        virtual ~Component() = default;
        Component(GameObject* owner);

        Component(const Component& other) = delete;
        Component(Component&& other) = delete;
        Component& operator=(const Component& other) = delete;
        Component& operator=(Component&& other) = delete;

        virtual void Update() {};
        virtual void Render() const {};

        void MarkForDeletion() { m_markedForDeletion = true; }
        bool IsMarkedForDeletion() const { return m_markedForDeletion; }


    protected:
       const GameObject* m_Owner;

    private:
        bool m_markedForDeletion{ false };
    };
}
    

