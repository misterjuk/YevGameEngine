#ifndef YEV_SERVICE_LOCATOR
#define YEV_SERVICE_LOCATOR

#include <memory>
#include "Singleton.h"
#include "SoundManager.h"

namespace yev
{

	class ServiceLocator final : public Singleton<ServiceLocator>
	{
	public:
		virtual ~ServiceLocator() = default;

		ServiceLocator(ServiceLocator const&) = delete;
		ServiceLocator(ServiceLocator&&) = delete;
		ServiceLocator& operator=(ServiceLocator const&) = delete;
		ServiceLocator& operator=(ServiceLocator&&) = delete;

		ISoundManager* GetSoundSystem();
		static void RegisterSoundSystem(std::unique_ptr<ISoundManager>&& pSoundSystem);
	private:
		friend class Singleton<ServiceLocator>;

		explicit ServiceLocator() = default;

		static std::unique_ptr<ISoundManager> m_pSoundSystem;
	};

}

#endif 