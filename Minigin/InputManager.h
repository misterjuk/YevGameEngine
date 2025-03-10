#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#include "Singleton.h"
#include <unordered_map>
#include <memory>
#include "Command.h"
#include <SDL.h>     // For SDL_Keycode and SDL functions



namespace yev
{
    class Gamepad;

    enum class InputState
    {
        Pressed,
        Released,
        Held
    };

    class InputManager final : public Singleton<InputManager>
    {
    public:
        InputManager();
        ~InputManager();

        bool ProcessInput();

        // Key binding for keyboard input
        void BindKeyboardCommand(SDL_Keycode key, InputState state, std::unique_ptr<Command> command);

        // Button binding for controller input
        void BindGamepadCommand(unsigned int button, InputState state, std::unique_ptr<Command> command);

    private:

        std::unordered_map<SDL_Keycode, std::pair<InputState, std::unique_ptr<Command>>> m_KeyboardCommands;
        std::unordered_map<SDL_Keycode, bool> m_PreviousKeyStates;

        bool IsPressed(SDL_Keycode key);

        std::unique_ptr<Gamepad> m_Gamepad;
    };
}

#endif