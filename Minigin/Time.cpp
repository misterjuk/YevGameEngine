#include "Time.h"

yev::Time::Time()
{
	m_LastTime = std::chrono::high_resolution_clock::now(); 
	m_DeltaTime = 0.0f;

}

float yev::Time::GetDeltaTime() const
{
	return m_DeltaTime;
}

void yev::Time::Update()
{
	const auto currentTime = std::chrono::high_resolution_clock::now();
	m_DeltaTime = std::chrono::duration<float>(currentTime - m_LastTime).count(); // In seconds
	m_LastTime = currentTime;
}
