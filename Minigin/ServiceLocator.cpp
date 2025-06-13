#include "ServiceLocator.h"
#include "SoundManager.h"


namespace yev {
	std::unique_ptr<ISoundManager> ServiceLocator::m_pSoundSystem{ std::make_unique<NullSoundManager>() };

	ISoundManager* ServiceLocator::GetSoundSystem()
	{
		return m_pSoundSystem.get();
	}

	void ServiceLocator::RegisterSoundSystem(std::unique_ptr<ISoundManager>&& pSoundSystem)
	{
		if (pSoundSystem)
		{
			m_pSoundSystem = std::move(pSoundSystem);
		}
		else
		{
			m_pSoundSystem = std::make_unique<NullSoundManager>();
		}
	}
}
