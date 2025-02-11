#pragma once
#include "Singleton.h"
#include <chrono>

namespace dae
{
	class Time final : public Singleton<Time>
	{

	public:

		float GetDeltaTime() const;


		void Update();
		void SetStartOfTheFrameTime();

		const float GetFixedTimeStep() const { return m_FixedTimeStep; };
		const int GetTargetFrameRate() const { return m_TargetFrameRate; };
		const int GetTargetMsPerFrame() const { return 1000 / m_TargetFrameRate; }
	private:

		const float m_FixedTimeStep{ 0.02f };
		const int m_TargetFrameRate{ 60 };

		float m_DeltaTime{};
		std::chrono::high_resolution_clock::time_point m_LastTime{};
	};
}

