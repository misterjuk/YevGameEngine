//INSPIRED FROM AdriaanMusschoot and Amugen https://github.com/AdriaanMusschoot/Amugen/tree/master

#ifndef YEV_SOUND_MANAGER_H
#define YEV_SOUND_MANAGER_H

#include <memory>
#include <map>
#include <string>
#include <filesystem>
#include <deque>
#include <mutex>
#include <future>
#include <thread>
#include "ISoundManager.h"
#include "SoundEffect.h"

namespace yev
{
    class SoundManager final : public ISoundManager
    {
    public:
        SoundManager();
        ~SoundManager() override;

        SoundManager(const SoundManager&) = delete;
        SoundManager& operator=(const SoundManager&) = delete;
        SoundManager(SoundManager&&) noexcept = delete;
        SoundManager& operator=(SoundManager&&) noexcept = delete;

        void ProcessSound();
        bool LoadSoundEffect(SoundId id, const std::string& filePath, int volume, int loops) override;
        bool RequestStopSoundEffect(SoundId id) override;

        void StartPlayback() override;
        void StopPlayback() override;

        void ToggleMute() override;

    private:
        struct SoundInfo {
            SoundId id;
            std::string filePath;
            int volume;
            int loops;
        };

        std::map<int, std::unique_ptr<SoundEffect>> m_SoundMap{};

        std::deque<SoundInfo> m_SoundInfoDeque{};
        std::deque<SoundId> m_SoundStopDeque{};

        std::thread m_SoundThread{};
        std::mutex m_SoundMutex{};
        std::promise<void> m_SoundPromise{};
        std::future<void> m_SoundFuture{};


        bool m_ShouldQuit{};

        bool m_IsScheduled{};
        bool m_IsMuted{};

        void PlaySoundEffect(SoundId id, const std::string&  fileName, int volume, int loops);
        void StopSoundEffect(SoundId id);
    };

    class LoggingSoundManager final : public ISoundManager
    {
    public:
        explicit LoggingSoundManager(std::unique_ptr<ISoundManager>&& realSoundManager);
        ~LoggingSoundManager() override = default;

        LoggingSoundManager(const LoggingSoundManager&) = delete;
        LoggingSoundManager& operator=(const LoggingSoundManager&) = delete;
        LoggingSoundManager(LoggingSoundManager&&) noexcept = delete;
        LoggingSoundManager& operator=(LoggingSoundManager&&) noexcept = delete;

        bool LoadSoundEffect(SoundId id, const std::string& filePath, int volume, int loops) override;
        bool RequestStopSoundEffect(SoundId id) override;

        void StartPlayback() override;
        void StopPlayback() override;

        void ToggleMute() override;

    private:
        std::unique_ptr<ISoundManager> m_pSoundManager;
    };

    class NullSoundManager final : public ISoundManager
    {
    public:
        NullSoundManager() = default;
        ~NullSoundManager() override = default;

        NullSoundManager(const NullSoundManager&) = delete;
        NullSoundManager& operator=(const NullSoundManager&) = delete;
        NullSoundManager(NullSoundManager&&) noexcept = delete;
        NullSoundManager& operator=(NullSoundManager&&) noexcept = delete;

        bool LoadSoundEffect(SoundId, const std::string&, int, int) override { return false; }
        bool RequestStopSoundEffect(SoundId) override { return false; }

        void StartPlayback() override {}
        void StopPlayback() override {}

        void ToggleMute() override {}
    };
} 

#endif 
