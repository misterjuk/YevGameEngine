#include "Time.h"

dae::Time::Time()
{
	m_LastTime = std::chrono::high_resolution_clock::now(); 
	m_DeltaTime = 0.0f;

}

float dae::Time::GetDeltaTime() const
{
	return m_DeltaTime;
}

void dae::Time::Update()
{
	const auto currentTime = std::chrono::high_resolution_clock::now();
	m_DeltaTime = std::chrono::duration<float>(currentTime - m_LastTime).count(); // In seconds
	m_LastTime = currentTime;
}
