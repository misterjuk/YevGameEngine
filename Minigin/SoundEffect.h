
//INSPIRED FROM AdriaanMusschoot and Amugen https://github.com/AdriaanMusschoot/Amugen/tree/master

#ifndef YEV_SOUND_EFFECT_H
#define YEV_SOUND_EFFECT_H

#include <string>

struct Mix_Chunk;

namespace yev {

    class SoundEffect final {
    public:
        explicit SoundEffect(const std::string& filePath);
        ~SoundEffect();

        SoundEffect(SoundEffect const&) = delete;
        SoundEffect(SoundEffect&&) noexcept = delete;
        SoundEffect& operator=(SoundEffect const&) = delete;
        SoundEffect& operator=(SoundEffect&&) noexcept = delete;

        void Play(int volume, int loopCount);
        void Stop();

    private:
        std::string m_FilePath;
        Mix_Chunk* m_pSoundEffect;
        int m_Channel;
    };

} 

#endif 
