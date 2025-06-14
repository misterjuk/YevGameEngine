#ifndef GAME_COMMANDS_H
#define GAME_COMMANDS_H

#include "Command.h"
#include "SceneManager.h"
#include "Scene.h"
#include "ServiceLocator.h"
#include "GameManager.h"
class StartGameCommand : public yev::Command
{
public:
    StartGameCommand() {};

    void Execute() override
    {
        if (yev::SceneManager::GetInstance().GetActiveScene().GetName() == "MainMenu")
        {
            GameManager::GetInstance().SkipToLevel(1);
        }
        else if (yev::SceneManager::GetInstance().GetActiveScene().GetName() == "EndScreen")
        {
            yev::SceneManager::GetInstance().SetActiveScene("MainMenu");
        }
    }

};

class MuteSoundCommand : public yev::Command
{
public:
    MuteSoundCommand() {};

    void Execute() override
    {
        yev::ServiceLocator::GetInstance().GetSoundSystem()->ToggleMute();

    }

};


class SkipLevelCommand : public yev::Command
{
public:
    SkipLevelCommand() = default;

    void Execute() override
    {
		//BAD WORKAROUND
        if (yev::SceneManager::GetInstance().GetActiveScene().GetName() == "MainMenu")
        {
            GameManager::GetInstance().SkipToLevel(1);
        }
        else 
        {
            int currentLevel = GameManager::GetInstance().GetCurrentLevel();
            GameManager::GetInstance().SkipToLevel(currentLevel + 1);
        }      
    }
};

#endif

