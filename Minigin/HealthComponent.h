#ifndef HEALTHCOMPONENT_H
#define HEALTHCOMPONENT_H

#include "IObserver.h"
#include "Component.h"
#include "Subject.h"

class GameObject;
class HealthComponent final : public yev::IObserver, public yev::Component, public yev::Subject
{
public:

	HealthComponent(yev::GameObject* owner);
	virtual ~HealthComponent() = default;

	HealthComponent(const HealthComponent& other) = delete;
	HealthComponent(HealthComponent&& other) = delete;
	HealthComponent& operator=(const HealthComponent& other) = delete;
	HealthComponent& operator=(HealthComponent&& other) = delete;

	void Notify(IObserver::Event event, yev::GameObject* actor) override;

	int GetHealth() const { return m_Health; }
private:

	int m_Health{ 3 };
};

#endif //  HEALTHCOMPONENT_H


