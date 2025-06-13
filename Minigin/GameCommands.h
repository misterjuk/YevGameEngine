#ifndef GAME_COMMANDS_H
#define GAME_COMMANDS_H

#include "Command.h"
#include "SceneManager.h"
#include "Scene.h"
#include "ServiceLocator.h"
class StartGameCommand : public yev::Command
{
public:
    StartGameCommand() {};

    void Execute() override
    {
        if (yev::SceneManager::GetInstance().GetActiveScene().GetName() == "MainMenu")
        {
            yev::SceneManager::GetInstance().SetActiveScene("Level1");
        }
    }

};

class RestartGameCommand : public yev::Command
{
public:
    RestartGameCommand() {};

    void Execute() override
    {
        if (yev::SceneManager::GetInstance().GetActiveScene().GetName() == "EndScreen")
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
#endif