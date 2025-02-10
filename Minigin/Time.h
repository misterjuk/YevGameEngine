#pragma once
#include "Singleton.h"


namespace dae
{
	class Time final : public Singleton<Time>
	{

	public:

		float GetDeltaTime() const;


		void Update();


	private:


	};
}

