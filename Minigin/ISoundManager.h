// INSPIRED FROM AdriaanMusschoot and Amugen https://github.com/AdriaanMusschoot/Amugen/tree/master

#ifndef YEV_ISOUND_MANAGER_H
#define YEV_ISOUND_MANAGER_H


#include <string>
namespace yev {

    using SoundId = int;

    class ISoundManager
    {
    public:
        virtual ~ISoundManager() = default;

        virtual bool LoadSoundEffect(SoundId id, const std::string& filePath, int volume, int loops) = 0;
        virtual bool RequestStopSoundEffect(SoundId id) = 0;

        virtual void StartPlayback() = 0;
        virtual void StopPlayback() = 0;

        virtual void ToggleMute() = 0;
    };
}
#endif