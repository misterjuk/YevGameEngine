
//INSPIRED FROM AdriaanMusschoot and Amugen https://github.com/AdriaanMusschoot/Amugen/tree/master

#include "SoundEffect.h"
#include <stdexcept>
#include "SDL_mixer.h"

namespace yev 
{
    SoundEffect::SoundEffect(std::string const& filePath)
        : m_FilePath{ filePath }, m_pSoundEffect{ nullptr }
    {
    
    }

    void SoundEffect::Play(int volume, int loops)
    {
        if (!m_pSoundEffect)
        {
            m_pSoundEffect = Mix_LoadWAV(m_FilePath.data());
            if (!m_pSoundEffect)
            {
                throw std::runtime_error("Failed to load sound effect: " + std::string(m_FilePath));
            }
        }

        Mix_VolumeChunk(m_pSoundEffect, volume);
        m_Channel = Mix_PlayChannel(-1, m_pSoundEffect, loops);
        if (m_Channel == -1)
        {
            throw std::runtime_error("Failed to play sound effect on channel.");
        }
    }

    void SoundEffect::Stop()
    {
        Mix_HaltChannel(m_Channel);
    }

    SoundEffect::~SoundEffect()
    {
        Mix_FreeChunk(m_pSoundEffect);
    }
}
