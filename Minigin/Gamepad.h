#ifndef GAMEPAD_H
#define GAMEPAD_H

#include "windows.h"
#include <XInput.h>
#include <unordered_map>
#include <memory>
#include "Command.h"
#include "InputManager.h"

namespace yev
{
    class Gamepad
    {
    public:
        Gamepad();
        ~Gamepad();

        void Update();
        void BindControllerCommand(unsigned int button, InputState state, std::unique_ptr<Command> command);

    private:
        std::unordered_map<unsigned int, std::pair<InputState, std::unique_ptr<Command>>> m_ControllerCommands;
        XINPUT_STATE m_ControllerState{};
        bool IsPressed(unsigned int button);

        std::unordered_map<unsigned int, bool> m_PreviousButtonStates;
    };
}

#endif // CONTROLLER_H