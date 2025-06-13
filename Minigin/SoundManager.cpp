//INSPIRED FROM AdriaanMusschoot and Amugen https://github.com/AdriaanMusschoot/Amugen/tree/master


#include "SoundManager.h"

#include "ResourceManager.h"
#include <SDL_mixer.h>
#include <iostream>
#include <queue>


namespace yev
{


SoundManager::SoundManager()
	: m_SoundPromise{ std::promise<void>() }
	, m_SoundFuture{ m_SoundPromise.get_future() }
{
	Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096);
	Mix_AllocateChannels(100);
}

SoundManager::~SoundManager()
{
	Mix_Quit();
	Mix_CloseAudio();
}

void SoundManager::ProcessSound()
{
	while (not m_ShouldQuit)
	{
		m_SoundFuture.get();

		std::deque<SoundInfo> soundDequeToPlay{};
		std::deque<SoundId> soundDequeToStop{};

		{
			std::lock_guard lock(m_SoundMutex);
			soundDequeToPlay = m_SoundInfoDeque;
			m_SoundInfoDeque.clear();
			soundDequeToStop = m_SoundStopDeque;
			m_SoundStopDeque.clear();

			m_SoundPromise = std::promise<void>();
			m_SoundFuture = m_SoundPromise.get_future();
			m_IsScheduled = false;
		}

		while (not soundDequeToPlay.empty())
		{
			auto [id, fileName, volume, loops] = soundDequeToPlay.front();
			PlaySoundEffect(id, fileName, volume, loops);
			soundDequeToPlay.pop_front();
		}

		while (not soundDequeToStop.empty())
		{
			int id = soundDequeToStop.front();
			StopSoundEffect(id);
			soundDequeToStop.pop_front();
		}
	}
}

bool SoundManager::LoadSoundEffect(SoundId id, const std::string& filePath, int volume, int loops)
{
	std::lock_guard lockPlaying{ m_SoundMutex };

	if (m_IsMuted)
	{
		return false;
	}

	if (bool hasFoundSimilar = std::any_of(m_SoundInfoDeque.begin(), m_SoundInfoDeque.end(),
		[&](SoundInfo& info)
		{
			if (info.id == id)
			{
				info.volume = std::max(info.volume, volume);
				return true;
			}
			return false;
		}); hasFoundSimilar)
	{
		return false;
	}

	m_SoundInfoDeque.emplace_back(SoundInfo{ id, filePath, volume, loops });

	if (not m_IsScheduled)
	{
		m_SoundPromise.set_value();
		m_IsScheduled = true;
	}

	return true;
}

bool SoundManager::RequestStopSoundEffect(SoundId id)
{
	std::lock_guard lockStopping{ m_SoundMutex };

	m_SoundStopDeque.emplace_back(id);
	if (not m_IsScheduled)
	{
		m_SoundPromise.set_value();
		m_IsScheduled = true;
	}

	return true;
}

void SoundManager::StartPlayback()
{
	std::lock_guard close{ m_SoundMutex };
	m_SoundThread = std::thread(&SoundManager::ProcessSound, this);
	m_SoundThread.detach();
}

void SoundManager::StopPlayback()
{
	std::lock_guard close{ m_SoundMutex };

	m_ShouldQuit = true;
	if (not m_IsScheduled)
	{
		m_SoundPromise.set_value();
		m_IsScheduled = true;
	}
}

void SoundManager::ToggleMute()
{
	std::lock_guard muteLock{ m_SoundMutex };

	m_IsMuted = not m_IsMuted;

	for (int channelIdx{}; channelIdx < MIX_CHANNELS; ++channelIdx)
	{
		int volume
		{
			[&]() -> int
			{
				if (m_IsMuted)
				{
					return 0;
				}
				else
				{
					return MIX_MAX_VOLUME;
				}
			}()
		};
		Mix_Volume(channelIdx, volume);
	}
}

void SoundManager::PlaySoundEffect(SoundId id, const std::string& fileName, int volume, int loops)
{
	if (not m_SoundMap.contains(id))
	{
		m_SoundMap[id] = ResourceManager::GetInstance().LoadSoundEffect(fileName);
	}

	m_SoundMap[id]->Play(volume, loops);
}

void SoundManager::StopSoundEffect(SoundId id)
{
	if (not m_SoundMap.contains(id))
	{
		return;
	}

	m_SoundMap[id]->Stop();
}


LoggingSoundManager::LoggingSoundManager(std::unique_ptr<ISoundManager>&& actualSoundSystemUPtr)
	: m_pSoundManager{ std::move(actualSoundSystemUPtr) }
{
}

bool LoggingSoundManager::LoadSoundEffect(SoundId id, const std::string& filePath, int volume, int loops)
{
	if (m_pSoundManager->LoadSoundEffect(id, filePath, volume, loops))
	{
		std::cout << "Requested sound id: " << id << " from dir " << filePath << "\n";
		std::cout << "Requested sound id: " << id << " at volume " << volume << "\n";
		return true;
	}
	return false;
}

bool LoggingSoundManager::RequestStopSoundEffect(SoundId id)
{
	if (m_pSoundManager->RequestStopSoundEffect(id))
	{
		std::cout << "Stop Sound Effect\n";
		return true;
	}
	return false;
}

void LoggingSoundManager::StartPlayback()
{
	m_pSoundManager->StartPlayback();
	std::cout << "Sound execution should start\n";
}

void LoggingSoundManager::StopPlayback()
{
	m_pSoundManager->StopPlayback();
	std::cout << "Sound execution should end\n";
}

void LoggingSoundManager::ToggleMute()
{
	m_pSoundManager->ToggleMute();
	std::cout << "Toggle soundthread muted\n";
}
}