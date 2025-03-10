#ifndef SUBJECT_H
#define SUBJECT_H

#include "IObserver.h"
#include <vector>

namespace yev
{
    class Subject
    {
    public:
        void AddObserver(IObserver* observer)
        {
            m_Observers.push_back(observer);
        }

        void RemoveObserver(IObserver* observer)
        {
            m_Observers.erase(std::remove(m_Observers.begin(), m_Observers.end(), observer), m_Observers.end());
        }

        void NotifyObservers(IObserver::Event event, GameObject* actor)
        {
            for (IObserver* observer : m_Observers)
            {
                observer->Notify(event, actor);
            }
        }

    private:
        std::vector<IObserver*> m_Observers;
    };
}

#endif // !SUBJECT_H
